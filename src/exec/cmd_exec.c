/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:45:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/02 17:40:55 by codespace        ###   ########.fr       */
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
	if (ft_strcmp(cmd->args[0], "!") == 0 && !cmd->args[1])
	{
		return (1);
	}
	if (is_builtin(cmd->args[0]))
	{
		builtin = get_builtin(cmd->args[0]);
		if (builtin)
		{
			stdin_backup = dup(STDIN_FILENO);
			stdout_backup = dup(STDOUT_FILENO);
			if (setup_redirections(cmd) != 0)
			{
				reset_redirections(stdin_backup, stdout_backup);
				return (1);
			}
			status = builtin(shell, cmd->args);
			reset_redirections(stdin_backup, stdout_backup);
			return (status);
		}
	}
	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
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
		if (ft_strchr(cmd->args[0], '/'))
		{
			struct stat file_stat;
			if (access(cmd->args[0], F_OK) == 0 && stat(cmd->args[0], &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
			{
				error_message(cmd->args[0], "Is a directory");
				exit(126);
			}
		}
		cmd_path = find_command_path(shell->path, cmd->args[0]);
		if (!cmd_path)
		{
			error_message(cmd->args[0], "No such file or directory");
			exit(127);
		}
		if (ft_strcmp(cmd_path, "PERMISSION_DENIED") == 0)
		{
			free(cmd_path);
			error_message(cmd->args[0], "Permission denied");
			exit(126);
		}
		char **env_array = env_list_to_array(shell->env);
		if (!env_array)
		{
			free(cmd_path);
			error_message(cmd->args[0], "Failed to create environment");
			exit(1);
		}
		execve(cmd_path, cmd->args, env_array);
		error_message(cmd->args[0], "Execution failed");

		// Clean up
		free(cmd_path);
		free_env_array(env_array);

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

static pid_t	execute_pipeline_command(t_shell *shell, t_cmds *cmd, int in_fd, int out_fd, int heredoc_fd)
{
	pid_t	pid;
	char	*cmd_path;
	t_builtin_func	builtin;
	int		effective_in_fd;

	if (!cmd->args || !cmd->args[0])
		return (-1);

	// Use heredoc_fd if available and this is the first command with heredoc
	effective_in_fd = (heredoc_fd >= 0 && cmd->heredoc_delimeter) ? heredoc_fd : in_fd;

	pid = fork();
	if (pid < 0)
		return (-1);

	if (pid == 0)
	{
		// Close all unused pipe file descriptors
		if (effective_in_fd != STDIN_FILENO)
		{
			dup2(effective_in_fd, STDIN_FILENO);
			close(effective_in_fd);
		}

		if (out_fd != STDOUT_FILENO)
		{
			dup2(out_fd, STDOUT_FILENO);
			close(out_fd);
		}

		// Close the original heredoc_fd if it's different from effective_in_fd
		if (heredoc_fd >= 0 && heredoc_fd != effective_in_fd)
			close(heredoc_fd);

		// Setup other redirections (but skip heredoc since it's already handled)
		if (cmd->input_file || cmd->output_file || cmd->rw_file)
		{
			t_cmds temp_cmd = *cmd;
			temp_cmd.heredoc_delimeter = NULL;  // Skip heredoc processing
			setup_redirections(&temp_cmd);
		}

		if (is_builtin(cmd->args[0]))
		{
			builtin = get_builtin(cmd->args[0]);
			if (builtin)
			{
				exit(builtin(shell, cmd->args));
			}
		}
		if (ft_strchr(cmd->args[0], '/'))
		{
			struct stat file_stat;
			if (access(cmd->args[0], F_OK) == 0 && stat(cmd->args[0], &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
			{
				error_message(cmd->args[0], "Is a directory");
				exit(126);
			}
		}
		cmd_path = find_command_path(shell->path, cmd->args[0]);
		if (!cmd_path)
		{
			error_message(cmd->args[0], "No such file or directory");
			exit(127);
		}
		if (ft_strcmp(cmd_path, "PERMISSION_DENIED") == 0)
		{
			free(cmd_path);
			error_message(cmd->args[0], "Permission denied");
			exit(126);
		}
		char **env_array = env_list_to_array(shell->env);
		if (!env_array)
		{
			free(cmd_path);
			error_message(cmd->args[0], "Failed to create environment");
			exit(1);
		}
		execve(cmd_path, cmd->args, env_array);
		error_message(cmd->args[0], "Execution failed");
		free(cmd_path);
		free_env_array(env_array);
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
	int		heredoc_fd = -1;

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
	current = commands;

	// Process heredoc for the first command if it has one
	if (current && current->heredoc_delimeter)
	{
		if (process_heredoc_for_pipeline(current, &heredoc_fd) != 0)
		{
			free(pids);
			return (1);
		}
		prev_pipe_read = heredoc_fd;
	}
	else
	{
		prev_pipe_read = STDIN_FILENO;
	}

	i = 0;
	while (current)
	{
		if (current->next)
		{
			if (pipe(pipefd) < 0)
			{
				error_message("pipe", "Failed to create pipe");
				if (heredoc_fd >= 0)
					close(heredoc_fd);
				free(pids);
				return (1);
			}
		}
		if (current->next)
		{
			pids[i] = execute_pipeline_command(shell, current, prev_pipe_read, pipefd[WRITE_END], heredoc_fd);
			if (prev_pipe_read != STDIN_FILENO && prev_pipe_read != heredoc_fd)
				close(prev_pipe_read);
			close(pipefd[WRITE_END]);
			prev_pipe_read = pipefd[READ_END];
		}
		else
		{
			pids[i] = execute_pipeline_command(shell, current, prev_pipe_read, STDOUT_FILENO, heredoc_fd);
			if (prev_pipe_read != STDIN_FILENO && prev_pipe_read != heredoc_fd)
				close(prev_pipe_read);
		}
		if (pids[i] == -1)
		{
			error_message(current->args[0], "Failed to execute command");
			// Clean up any remaining file descriptors
			if (prev_pipe_read != STDIN_FILENO && prev_pipe_read != heredoc_fd)
				close(prev_pipe_read);
			if (heredoc_fd >= 0)
				close(heredoc_fd);
			free(pids);
			return (1);
		}
		current = current->next;
		i++;
		// Clear heredoc_fd for subsequent commands
		if (i == 1 && heredoc_fd >= 0)
		{
			close(heredoc_fd);
			heredoc_fd = -1;
		}
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
