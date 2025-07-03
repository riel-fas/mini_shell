/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   advanced_pipeline.c                              :+:      :+:    :+:   */
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
 * Advanced pipeline execution with comprehensive process management
 * Handles complex pipelines with proper file descriptor management
 */
int	execute_pipeline(t_shell *shell, t_cmds *commands)
{
	t_cmds	*current;
	pid_t	*pids;
	int		cmd_count;
	int		i;
	int		status;
	int		prev_pipe_read;
	int		heredoc_fd;

	if (!commands)
		return (0);

	cmd_count = count_pipeline_commands(commands);
	pids = (pid_t *)malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
	{
		ft_cmd_error("malloc", "Failed to allocate memory for pipeline", 1);
		return (1);
	}

	heredoc_fd = prepare_pipeline_heredoc(commands);
	prev_pipe_read = (heredoc_fd >= 0) ? heredoc_fd : STDIN_FILENO;

	current = commands;
	i = 0;

	while (current)
	{
		pids[i] = execute_pipeline_command_advanced(shell, current,
			&prev_pipe_read, i, cmd_count, heredoc_fd);

		if (pids[i] == -1)
		{
			cleanup_pipeline(pids, i, prev_pipe_read, heredoc_fd);
			return (1);
		}

		current = current->next;
		i++;

		// Clear heredoc_fd after first command
		if (i == 1 && heredoc_fd >= 0)
		{
			close(heredoc_fd);
			heredoc_fd = -1;
		}
	}

	// Wait for all processes and get final status
	status = wait_for_pipeline(pids, cmd_count);
	free(pids);

	return (status);
}

/*
 * Counts the number of commands in a pipeline
 * Returns the total number of commands to execute
 */
int	count_pipeline_commands(t_cmds *commands)
{
	t_cmds	*current;
	int		count;

	count = 0;
	current = commands;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/*
 * Prepares heredoc for the first command in pipeline if needed
 * Returns heredoc file descriptor or -1 if none
 */
int	prepare_pipeline_heredoc(t_cmds *commands)
{
	int	heredoc_fd;

	heredoc_fd = -1;
	if (commands && commands->heredoc_delimeter)
	{
		if (process_heredoc_for_pipeline(commands, &heredoc_fd) != 0)
			return (-2); // Error indicator
	}
	return (heredoc_fd);
}

/*
 * Executes a single command in a pipeline with proper pipe management
 * Handles input/output redirection between pipeline stages
 */
pid_t	execute_pipeline_command_advanced(t_shell *shell, t_cmds *cmd,
	int *prev_pipe_read, int cmd_index, int cmd_count, int heredoc_fd)
{
	int		pipefd[2];
	pid_t	pid;
	int		effective_input;

	(void)cmd_count; // Suppress unused parameter warning

	// Create pipe for all commands except the last
	if (cmd->next)
	{
		if (!create_pipe_safe(pipefd))
			return (-1);
	}

	// Determine effective input (heredoc takes precedence for first command)
	effective_input = (heredoc_fd >= 0 && cmd_index == 0 && cmd->heredoc_delimeter)
		? heredoc_fd : *prev_pipe_read;

	pid = fork();
	if (pid < 0)
		return (-1);

	if (pid == 0)
	{
		// Child process
		execute_pipeline_child(shell, cmd, effective_input, pipefd, cmd->next != NULL);
	}
	else
	{
		// Parent process
		handle_pipeline_parent(prev_pipe_read, pipefd, cmd, effective_input, heredoc_fd);
	}

	return (pid);
}

/*
 * Handles child process execution in pipeline
 * Sets up file descriptors and executes the command
 */
void	execute_pipeline_child(t_shell *shell, t_cmds *cmd, int input_fd,
	int *pipefd, int has_next)
{
	char			*cmd_path;
	char			**env_array;
	t_builtin_func	builtin;

	// Setup input redirection
	if (input_fd != STDIN_FILENO)
	{
		if (dup2(input_fd, STDIN_FILENO) == -1)
			exit(1);
		close(input_fd);
	}

	// Setup output redirection for pipes
	if (has_next)
	{
		if (dup2(pipefd[WRITE_END], STDOUT_FILENO) == -1)
			exit(1);
		close(pipefd[READ_END]);
		close(pipefd[WRITE_END]);
	}

	// Handle file redirections (but skip heredoc since it's already handled)
	if (handle_file_redirections_only(cmd) != 0)
		exit(1);

	setup_child_signals();

	// Execute builtin or external command
	if (is_builtin(cmd->args[0]))
	{
		builtin = get_builtin(cmd->args[0]);
		if (builtin)
			exit(builtin(shell, cmd->args));
	}

	// Execute external command
	cmd_path = get_command_path(cmd->args[0], shell->env);
	if (!cmd_path)
	{
		ft_cmd_error(cmd->args[0], "command not found", 127);
		exit(127);
	}

	env_array = convert_env_to_array(shell->env);
	if (!env_array)
	{
		free(cmd_path);
		exit(1);
	}

	execve(cmd_path, cmd->args, env_array);

	handle_execution_error(cmd_path, cmd->args[0]);
	free(cmd_path);
	free_env_array(env_array);
	exit(1);
}

/*
 * Handles parent process responsibilities in pipeline
 * Manages file descriptor cleanup and pipe setup
 */
void	handle_pipeline_parent(int *prev_pipe_read, int *pipefd, t_cmds *cmd,
	int effective_input, int heredoc_fd)
{
	(void)effective_input; // Suppress unused parameter warning
	(void)heredoc_fd; // Suppress unused parameter warning

	// Close previous pipe read end if it's not stdin and not heredoc
	if (*prev_pipe_read != STDIN_FILENO && *prev_pipe_read != heredoc_fd)
		close(*prev_pipe_read);

	// Setup for next command
	if (cmd->next)
	{
		close(pipefd[WRITE_END]);
		*prev_pipe_read = pipefd[READ_END];
	}
}

/*
 * Handles file redirections only (not heredoc)
 * Used in pipeline where heredoc is handled separately
 */
int	handle_file_redirections_only(t_cmds *cmd)
{
	int	fd;

	// Handle input file (overrides heredoc/pipe input)
	if (cmd->input_file)
	{
		fd = open_input_file(cmd->input_file);
		if (fd < 0)
			return (1);
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			close(fd);
			return (1);
		}
		close(fd);
	}

	// Handle output file
	if (cmd->output_file)
	{
		fd = open_output_file(cmd->output_file, cmd->append_node);
		if (fd < 0)
			return (1);
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			close(fd);
			return (1);
		}
		close(fd);
	}

	// Handle read-write file
	if (cmd->rw_file)
	{
		fd = open(cmd->rw_file, O_RDWR | O_CREAT, 0644);
		if (fd < 0)
			return (1);
		if (dup2(fd, STDIN_FILENO) < 0 || dup2(fd, STDOUT_FILENO) < 0)
		{
			close(fd);
			return (1);
		}
		close(fd);
	}

	return (0);
}

/*
 * Waits for all pipeline processes and returns final status
 * Returns the exit status of the last command in the pipeline
 */
int	wait_for_pipeline(pid_t *pids, int cmd_count)
{
	int		status;
	int		final_status;
	int		i;
	pid_t	wpid;

	setup_parent_waiting_signals();
	final_status = 0;

	// Wait for all processes
	for (i = 0; i < cmd_count; i++)
	{
		wpid = waitpid(pids[i], &status, 0);
		if (wpid == pids[cmd_count - 1]) // Last command
		{
			if (WIFEXITED(status))
				final_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGINT)
					final_status = 130;
				else if (WTERMSIG(status) == SIGQUIT)
					final_status = 131;
				else
					final_status = 128 + WTERMSIG(status);
			}
		}
	}

	restore_signals();
	return (final_status);
}

/*
 * Cleans up pipeline resources on error
 * Closes file descriptors and frees allocated memory
 */
void	cleanup_pipeline(pid_t *pids, int created_processes, int prev_pipe_read, int heredoc_fd)
{
	int	i;

	// Close any open file descriptors
	if (prev_pipe_read != STDIN_FILENO && prev_pipe_read != heredoc_fd)
		close(prev_pipe_read);
	if (heredoc_fd >= 0)
		close(heredoc_fd);

	// Wait for any created processes
	for (i = 0; i < created_processes; i++)
	{
		if (pids[i] > 0)
			waitpid(pids[i], NULL, 0);
	}

	free(pids);
}
