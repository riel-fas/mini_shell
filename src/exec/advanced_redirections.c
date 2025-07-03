/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   advanced_redirections.c                           :+:      :+:    :+:   */
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
 * Opens input file with proper error handling
 * Returns file descriptor on success, -1 on failure
 */
int	open_input_file(char *file)
{
	int	fd;

	if (!file)
		return (-1);

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		ft_cmd_error(file, "No such file or directory\n", 1);
		return (-1);
	}
	return (fd);
}

/*
 * Opens output file with proper flags and error handling
 * Returns file descriptor on success, -1 on failure
 */
int	open_output_file(char *file, int append)
{
	int	fd;
	int	flags;

	if (!file)
		return (-1);

	if (append)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;

	fd = open(file, flags, 0644);
	if (fd < 0)
	{
		ft_cmd_error(file, "Permission denied\n", 1);
		return (-1);
	}
	return (fd);
}

/*
 * Advanced redirection handling for all types of redirections
 * Handles input, output, append, and heredoc redirections
 */
int	handle_exec_redirections(t_cmds *cmd)
{
	int	fd;

	if (!cmd)
		return (0);

	// Handle heredoc first (highest priority for input)
	if (cmd->heredoc_fd >= 0)
	{
		if (dup2(cmd->heredoc_fd, STDIN_FILENO) < 0)
			return (1);
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	else if (cmd->heredoc_list)
	{
		if (process_heredoc_list(cmd) != 0)
			return (1);
	}

	// Handle input redirection (overrides heredoc if both exist)
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

	// Handle output redirection
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

	// Handle read-write redirection
	if (cmd->rw_file)
	{
		fd = open(cmd->rw_file, O_RDWR | O_CREAT, 0644);
		if (fd < 0)
		{
			ft_cmd_error(cmd->rw_file, "Cannot open file\n", 1);
			return (1);
		}
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
 * Processes multiple heredocs in sequence
 * Only the last heredoc's content is used for input
 */
int	process_heredoc_list(t_cmds *cmd)
{
	t_heredoc_list	*current;
	int				dummy_fd;
	t_cmds			temp_cmd;

	if (!cmd->heredoc_list)
		return (0);

	current = cmd->heredoc_list;
	while (current)
	{
		// Create temporary command for each heredoc
		ft_memset(&temp_cmd, 0, sizeof(t_cmds));
		temp_cmd.heredoc_delimeter = current->delimiter;

		if (current->next == NULL)
		{
			// Last heredoc - keep its file descriptor
			if (create_heredoc_pipe(&temp_cmd, &cmd->heredoc_fd) != 0)
				return (1);
		}
		else
		{
			// Not the last heredoc - process but discard
			if (create_heredoc_pipe(&temp_cmd, &dummy_fd) != 0)
				return (1);
			if (dummy_fd >= 0)
				close(dummy_fd);
		}
		current = current->next;
	}

	return (0);
}

/*
 * Advanced heredoc processing with proper signal handling
 * Creates a pipe and forks a child to read heredoc input
 */
int	create_heredoc_pipe(t_cmds *cmd, int *heredoc_fd)
{
	int		pipe_fds[2];
	pid_t	pid;
	char	*line;
	int		status;

	if (!cmd->heredoc_delimeter)
		return (0);

	if (pipe(pipe_fds) < 0)
		return (1);

	pid = fork();
	if (pid < 0)
	{
		close(pipe_fds[READ_END]);
		close(pipe_fds[WRITE_END]);
		return (1);
	}

	if (pid == 0)
	{
		// Child process - read heredoc input
		close(pipe_fds[READ_END]);
		setup_heredoc_signals();

		while (1)
		{
			ft_putstr_fd("> ", STDERR_FILENO);
			line = get_next_line(STDIN_FILENO);

			if (!line) // EOF (Ctrl+D)
			{
				ft_putchar_fd('\n', STDERR_FILENO);
				close(pipe_fds[WRITE_END]);
				exit(0);
			}

			// Remove trailing newline for comparison
			if (line[ft_strlen(line) - 1] == '\n')
				line[ft_strlen(line) - 1] = '\0';

			if (ft_strcmp(line, cmd->heredoc_delimeter) == 0)
			{
				free(line);
				close(pipe_fds[WRITE_END]);
				exit(0);
			}

			// Write line back with newline
			ft_putstr_fd(line, pipe_fds[WRITE_END]);
			ft_putchar_fd('\n', pipe_fds[WRITE_END]);
			free(line);
		}
	}
	else
	{
		// Parent process - wait for child
		close(pipe_fds[WRITE_END]);
		setup_parent_heredoc_signals();
		waitpid(pid, &status, 0);
		restore_heredoc_signals();

		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			close(pipe_fds[READ_END]);
			g_exit_status = 130;
			return (130);
		}

		*heredoc_fd = pipe_fds[READ_END];
		return (0);
	}
}

/*
 * Sets up all redirections for a command with backup handling
 * Returns 0 on success, non-zero on failure
 */
int	setup_redirections(t_cmds *cmd)
{
	int	stdin_backup;
	int	stdout_backup;
	int	status;

	if (!cmd)
		return (0);

	// No redirections needed
	if (!cmd->input_file && !cmd->output_file && !cmd->rw_file
		&& !cmd->heredoc_delimeter && cmd->heredoc_fd < 0 && !cmd->heredoc_list)
		return (0);

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);

	status = handle_exec_redirections(cmd);

	if (status != 0)
	{
		reset_redirections(stdin_backup, stdout_backup);
		return (status);
	}

	close(stdin_backup);
	close(stdout_backup);
	return (0);
}

/*
 * Resets file descriptors to their original state
 * Used for cleanup after redirection handling
 */
void	reset_redirections(int stdin_backup, int stdout_backup)
{
	if (stdin_backup >= 0)
	{
		dup2(stdin_backup, STDIN_FILENO);
		close(stdin_backup);
	}

	if (stdout_backup >= 0)
	{
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdout_backup);
	}
}

/*
 * Process all heredocs for pipeline command
 * Returns 0 on success, error code on failure
 */
int	process_heredoc_for_pipeline(t_cmds *cmd, int *heredoc_fd)
{
	if (!cmd->heredoc_delimeter)
	{
		*heredoc_fd = -1;
		return (0);
	}
	return (create_heredoc_pipe(cmd, heredoc_fd));
}

/*
 * Process all heredocs after parsing stage
 * Creates file descriptors for all heredoc inputs
 * Returns 0 on success, error code on failure
 */
int	process_heredocs_after_parsing(t_cmds *commands)
{
	t_cmds	*current;

	current = commands;
	while (current)
	{
		if (current->heredoc_list)
		{
			// For multiple heredocs, process them in sequence
			if (process_all_heredocs(current) != 0)
				return (1);
		}
		else if (current->heredoc_delimeter)
		{
			if (create_heredoc_pipe(current, &current->heredoc_fd) != 0)
				return (1);
		}
		current = current->next;
	}
	return (0);
}

/*
 * Process multiple heredocs in sequence
 * Only the last one is kept for input
 * Returns 0 on success, error code on failure
 */
int	process_all_heredocs(t_cmds *cmd)
{
	t_heredoc_list	*current;
	int				dummy_fd;

	if (!cmd->heredoc_list)
		return (0);

	current = cmd->heredoc_list;
	while (current)
	{
		// Create a temporary command structure for each heredoc
		t_cmds temp_cmd = *cmd;
		temp_cmd.heredoc_delimeter = current->delimiter;

		if (current->next == NULL)
		{
			// This is the last heredoc - keep its file descriptor
			if (create_heredoc_pipe(&temp_cmd, &cmd->heredoc_fd) != 0)
				return (1);
		}
		else
		{
			// This is not the last heredoc - process but discard
			if (create_heredoc_pipe(&temp_cmd, &dummy_fd) != 0)
				return (1);
			if (dummy_fd >= 0)
				close(dummy_fd);
		}
		current = current->next;
	}

	return (0);
}
