/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   advanced_executor.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 14:30:00 by riad              #+#    #+#             */
/*   Updated: 2025/07/03 14:30:00 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

extern int	g_exit_status;

/*
 * Creates a pipe with error handling
 * Returns 1 on success, 0 on failure
 */
int	create_pipe_safe(int *pipe_fds)
{
	if (pipe(pipe_fds) == -1)
	{
		ft_cmd_error(NULL, "pipe error\n", 1);
		g_exit_status = 1;
		return (0);
	}
	return (1);
}

/*
 * Advanced execution function that handles pipelines with proper process management
 * Main execution function that creates child processes for each command
 * in the pipeline, sets up pipes between them, and coordinates execution
 */
int	execution(t_cmds *commands, t_env *env)
{
	int		pipe_fds[2];
	pid_t	pid;
	int		input_fd;
	pid_t	last_pid;
	t_cmds	*current;

	if (!commands)
		return (0);

	input_fd = 0;
	current = commands;
	last_pid = -1;

	while (current)
	{
		// Create pipe for all commands except the last one
		if (current->next)
		{
			if (!create_pipe_safe(pipe_fds))
				return (0);
		}

		pid = fork();
		if (pid == -1)
		{
			g_exit_status = 1;
			perror("minishell: fork");
			return (0);
		}

		if (pid == 0)
		{
			// Child process
			ft_child_process(current, env, input_fd, pipe_fds);
		}
		else
		{
			// Parent process
			last_pid = pid;
			ft_parent_process(&input_fd, pipe_fds, current);
		}

		current = current->next;
	}

	// Wait for all processes and get exit status
	finish_execution(last_pid);
	return (1);
}

/*
 * Handles child process setup for pipeline execution
 * Sets up file descriptors for input/output pipes and redirections
 */
void	ft_child_process(t_cmds *cmd, t_env *env, int input_fd, int *pipe_fds)
{
	char	*cmd_path;
	char	**env_array;

	// Setup input redirection
	if (input_fd != 0)
	{
		if (dup2(input_fd, STDIN_FILENO) == -1)
			exit(1);
		close(input_fd);
	}

	// Setup output redirection for pipes
	if (cmd->next)
	{
		if (dup2(pipe_fds[WRITE_END], STDOUT_FILENO) == -1)
			exit(1);
		close(pipe_fds[READ_END]);
		close(pipe_fds[WRITE_END]);
	}

	// Handle file redirections
	if (handle_exec_redirections(cmd) != 0)
		exit(1);

	// Setup child signals
	setup_child_signals();

	// Check if it's a builtin command
	if (is_builtin(cmd->args[0]))
	{
		t_builtin_func builtin = get_builtin(cmd->args[0]);
		if (builtin)
		{
			// Create a temporary shell structure for builtins
			t_shell temp_shell;
			temp_shell.env = env;
			temp_shell.exit_status = g_exit_status;
			exit(builtin(&temp_shell, cmd->args));
		}
	}

	// Execute external command
	cmd_path = get_command_path(cmd->args[0], env);
	if (!cmd_path)
	{
		ft_cmd_error(cmd->args[0], "command not found\n", 127);
		exit(127);
	}

	env_array = convert_env_to_array(env);
	if (!env_array)
	{
		free(cmd_path);
		exit(1);
	}

	execve(cmd_path, cmd->args, env_array);

	// If execve fails
	handle_execution_error(cmd_path, cmd->args[0]);
	free(cmd_path);
	free_env_array(env_array);
	exit(1);
}

/*
 * Handles parent process responsibilities in pipeline execution
 * Manages file descriptor cleanup and pipe setup for next command
 */
void	ft_parent_process(int *input_fd, int *pipe_fds, t_cmds *cmd)
{
	if (*input_fd != 0)
		close(*input_fd);

	if (cmd->next)
	{
		close(pipe_fds[WRITE_END]);
		*input_fd = pipe_fds[READ_END];
	}
}

/*
 * Waits for all child processes and sets the global exit status
 * based on the last command in the pipeline
 */
void	finish_execution(pid_t last_pid)
{
	int		status;
	pid_t	wpid;
	int		last_status = 0;

	setup_parent_waiting_signals();

	// Wait for all child processes
	while ((wpid = wait(&status)) > 0)
	{
		if (wpid == last_pid)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGINT)
					last_status = 130;
				else if (WTERMSIG(status) == SIGQUIT)
					last_status = 131;
				else
					last_status = 128 + WTERMSIG(status);
			}
		}
	}

	restore_signals();
	g_exit_status = last_status;
}
