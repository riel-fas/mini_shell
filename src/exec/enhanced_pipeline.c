/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enhanced_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 12:29:00 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 17:59:14 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"
#include "../../includes/executor.h"

/**
 * Creates a pipe with error handling
 * @param pipe_fds Array to store pipe file descriptors
 * @return 1 on success, 0 on failure
 */
int	create_pipe_enhanced(int *pipe_fds)
{
	if (pipe(pipe_fds) == -1)
	{
		ft_cmd_error(NULL, "pipe creation failed", 1);
		return (0);
	}
	return (1);
}

/**
 * Handle the failure of a fork operation
 * @param cmd The command that was being processed
 * @return The appropriate error code
 */
int	handle_fork_failure(t_cmds *cmd)
{
	ft_cmd_error(cmd->args[0], "fork failed: resource temporarily unavailable", 1);
	return (1);
}

/**
 * Safely close a file descriptor if it's valid
 * @param fd File descriptor to close
 */
void	safe_close(int fd)
{
	if (fd > 2)
		close(fd);
}

/**
 * Handle zombie processes by waiting for all child processes
 * @param pids Array of process IDs
 * @param count Number of processes
 * @return Exit status of the last command
 */
int	wait_for_all_children(pid_t *pids, int count)
{
	int	status;
	int	last_status;
	int	i;

	i = 0;
	status = 0;
	last_status = 0;

	while (i < count)
	{
		if (waitpid(pids[i], &status, 0) > 0)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
		i++;
	}

	return (last_status);
}

/**
 * Enhanced pipeline execution with better error handling
 * @param shell Shell context
 * @param commands List of commands in the pipeline
 * @return Exit status of the last command
 */
int	execute_pipeline_enhanced(t_shell *shell, t_cmds *commands)
{
	int		cmd_count;
	int		i;
	int		pipefd[2];
	int		prev_pipe_read;
	pid_t	*pids;

	// Count commands in pipeline
	cmd_count = 0;
	t_cmds *temp = commands;
	while (temp)
	{
		cmd_count++;
		temp = temp->next;
	}

	// Allocate memory for process IDs
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
	{
		ft_cmd_error(NULL, "memory allocation failed", 1);
		return (1);
	}

	// Process all heredocs first
	if (!prepare_pipeline_heredoc(commands))
	{
		free(pids);
		return (g_exit_status);
	}

	// Execute the pipeline
	i = 0;
	prev_pipe_read = 0;
	temp = commands;

	setup_parent_waiting_signals();

	while (temp && i < cmd_count)
	{
		if (temp->next && !create_pipe_enhanced(pipefd))
		{
			while (i > 0)
				safe_close(prev_pipe_read);
			free(pids);
			return (1);
		}

		pids[i] = fork();
		if (pids[i] < 0)
		{
			if (temp->next)
			{
				safe_close(pipefd[0]);
				safe_close(pipefd[1]);
			}
			handle_fork_failure(temp);
			break;
		}

		if (pids[i] == 0)
		{
			// Child process
			if (temp->next)
				safe_close(pipefd[0]);

			execute_pipeline_child(shell, temp, prev_pipe_read, pipefd, temp->next != NULL);
			exit(g_exit_status); // Child always exits
		}

		// Parent process
		if (i > 0)
			safe_close(prev_pipe_read);

		if (temp->next)
		{
			safe_close(pipefd[1]);
			prev_pipe_read = pipefd[0];
		}

		temp = temp->next;
		i++;
	}

	// Wait for all child processes
	int final_status = wait_for_all_children(pids, i);

	free(pids);
	restore_signals();

	return (final_status);
}

/**
 * Verify that all commands in a pipeline have a valid executable
 * or are built-ins before execution
 * @param shell Shell context
 * @param commands List of commands in the pipeline
 * @return 1 if all commands are valid, 0 otherwise
 */
int	verify_pipeline_commands(t_shell *shell, t_cmds *commands)
{
	t_cmds	*cmd;
	char	*cmd_path;
	int		valid_cmds;

	valid_cmds = 1;
	cmd = commands;

	while (cmd)
	{
		// Empty command or just redirections
		if (!cmd->args || !cmd->args[0])
		{
			cmd = cmd->next;
			continue;
		}

		// Check if it's a built-in
		if (is_builtin(cmd->args[0]))
		{
			cmd = cmd->next;
			continue;
		}

		// Check if it's a valid external command
		cmd_path = get_command_path_enhanced(cmd->args[0], shell->env);
		if (!cmd_path)
		{
			valid_cmds = 0;
			// We don't break here to catch all command errors
		}
		else
			free(cmd_path);

		cmd = cmd->next;
	}

	return (valid_cmds);
}
