/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:45:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/14 00:52:16 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

int	execute_single_command(t_shell *shell, t_cmds *cmd)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;
	int		stdin_backup;
	int		stdout_backup;
	t_builtin_func	builtin;

	if (!cmd->args || !cmd->args[0])
		return (0);

	// Handle standalone logical operators that should be syntax errors
	if (ft_strcmp(cmd->args[0], "!") == 0 && !cmd->args[1])
	{
		// Standalone '!' should exit with status 1 (like bash) without error message
		return (1);
	}

	// Check for built-in commands
	if (is_builtin(cmd->args[0]))
	{
		builtin = get_builtin(cmd->args[0]);
		if (builtin)
		{
			// Save standard input/output
			stdin_backup = dup(STDIN_FILENO);
			stdout_backup = dup(STDOUT_FILENO);

			// Setup redirections
			if (setup_redirections(cmd) != 0)
			{
				reset_redirections(stdin_backup, stdout_backup);
				return (1);
			}

			// Execute built-in
			status = builtin(shell, cmd->args);

			// Reset redirections
			reset_redirections(stdin_backup, stdout_backup);
			return (status);
		}
	}

	// Save current stdin/stdout
	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);

	// Setup redirections if any
	if (setup_redirections(cmd) != 0)
	{
		reset_redirections(stdin_backup, stdout_backup);
		return (1);
	}

	pid = fork();
	if (pid < 0)
	{
		reset_redirections(stdin_backup, stdout_backup);
		error_message(cmd->args[0], "Fork failed");
		return (1);
	}
	else if (pid == 0)
	{
		// Child process
		// Find command path
		cmd_path = find_command_path(shell->path, cmd->args[0]);
		if (!cmd_path)
		{
			error_message(cmd->args[0], "Command not found");
			exit(127);
		}

		// Create environment array from linked list
		char **env_array = env_list_to_array(shell->env);
		if (!env_array)
		{
			free(cmd_path);
			error_message(cmd->args[0], "Failed to create environment");
			exit(1);
		}

		// Execute command
		execve(cmd_path, cmd->args, env_array);

		// If execve returns, it failed
		error_message(cmd->args[0], "Execution failed");

		// Clean up
		free(cmd_path);
		while (*env_array)
			free(*env_array++);
		free(env_array);

		exit(1);
	}
	else
	{
		// Parent process
		waitpid(pid, &status, 0);
		reset_redirections(stdin_backup, stdout_backup);

		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else
			return (1);
	}
}

static pid_t	execute_pipeline_command(t_shell *shell, t_cmds *cmd,
                                        int in_fd, int out_fd)
{
	pid_t	pid;
	char	*cmd_path;
	t_builtin_func	builtin;

	if (!cmd->args || !cmd->args[0])
		return (-1);

	// Fork even for built-ins in a pipeline
	pid = fork();
	if (pid < 0)
		return (-1);

	if (pid == 0)
	{
		// Child process

		// Connect pipes
		if (in_fd != STDIN_FILENO)
		{
			dup2(in_fd, STDIN_FILENO);
			close(in_fd);
		}

		if (out_fd != STDOUT_FILENO)
		{
			dup2(out_fd, STDOUT_FILENO);
			close(out_fd);
		}

		// Setup redirections
		setup_redirections(cmd);

		// Check for built-ins
		if (is_builtin(cmd->args[0]))
		{
			builtin = get_builtin(cmd->args[0]);
			if (builtin)
			{
				exit(builtin(shell, cmd->args));
			}
		}

		// External command
		cmd_path = find_command_path(shell->path, cmd->args[0]);
		if (!cmd_path)
		{
			error_message(cmd->args[0], "Command not found");
			exit(127);
		}

		// Create environment array
		char **env_array = env_list_to_array(shell->env);
		if (!env_array)
		{
			free(cmd_path);
			error_message(cmd->args[0], "Failed to create environment");
			exit(1);
		}

		execve(cmd_path, cmd->args, env_array);

		// If execve returns, it failed
		error_message(cmd->args[0], "Execution failed");
		exit(1);
	}

	return (pid);
}

int	execute_pipeline(t_shell *shell, t_cmds *commands)
{
	t_cmds	*current;
	int		pipefd[2];
	pid_t	*pids;
	int		cmd_count;
	int		i;
	int		status;
	int		prev_pipe_read;

	// Count commands and allocate PIDs array
	cmd_count = 0;
	current = commands;
	while (current)
	{
		cmd_count++;
		current = current->next;
	}

	pids = (pid_t *)malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
	{
		error_message("malloc", "Failed to allocate memory for pipeline");
		return (1);
	}

	// Execute commands in pipeline
	current = commands;
	prev_pipe_read = STDIN_FILENO;
	i = 0;

	while (current)
	{
		// Create a pipe except for the last command
		if (current->next)
		{
			if (pipe(pipefd) < 0)
			{
				error_message("pipe", "Failed to create pipe");
				free(pids);
				return (1);
			}
		}

		// Execute the command
		if (current->next)
		{
			// Not the last command - connect to pipe
			pids[i] = execute_pipeline_command(shell, current, prev_pipe_read, pipefd[WRITE_END]);

			// Close the pipe ends we've passed to the child
			if (prev_pipe_read != STDIN_FILENO)
				close(prev_pipe_read);
			close(pipefd[WRITE_END]);

			// Save read end for next command
			prev_pipe_read = pipefd[READ_END];
		}
		else
		{
			// Last command - output to stdout
			pids[i] = execute_pipeline_command(shell, current, prev_pipe_read, STDOUT_FILENO);

			// Close the pipe read end we've passed to the child
			if (prev_pipe_read != STDIN_FILENO)
				close(prev_pipe_read);
		}

		// Check for error
		if (pids[i] == -1)
		{
			error_message(current->args[0], "Failed to execute command");
			free(pids);
			return (1);
		}

		current = current->next;
		i++;
	}

	// Wait for all child processes
	for (i = 0; i < cmd_count; i++)
	{
		waitpid(pids[i], &status, 0);
	}

	free(pids);

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (1);
}
