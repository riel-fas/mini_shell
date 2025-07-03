/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:45:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/03 02:34:48 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

static int	setup_input_redirection(t_cmds *cmd)
{
	int	fd;

	if (!cmd->input_file)
		return (0);

	fd = open(cmd->input_file, O_RDONLY);
	if (fd < 0)
	{
		error_message(cmd->input_file, "No such file or directory");
		return (1);
	}

	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	setup_output_redirection(t_cmds *cmd)
{
	int	fd;
	int	flags;

	if (!cmd->output_file)
		return (0);

	if (cmd->append_node)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;

	fd = open(cmd->output_file, flags, 0644);
	if (fd < 0)
	{
		error_message(cmd->output_file, "Cannot open output file");
		return (1);
	}

	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	setup_rw_redirection(t_cmds *cmd)
{
	int	fd;

	if (!cmd->rw_file)
		return (0);

	fd = open(cmd->rw_file, O_RDWR | O_CREAT, 0644);
	if (fd < 0)
	{
		error_message(cmd->rw_file, "No such file or directory");
		return (1);
	}

	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return (1);
	}

	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	create_heredoc_pipe(t_cmds *cmd, int *heredoc_fd)
{
	int		pipe_fds[2];
	char	*line;
	int		status;
	pid_t	pid;

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
		close(pipe_fds[READ_END]);
		setup_heredoc_signals();

		while (1)
		{
			write(1, "> ", 2);
			fsync(1);
			line = get_next_line(0);

			if (!line) // EOF (Ctrl+D)
			{
				write(1, "\n", 1);
				close(pipe_fds[WRITE_END]);
				exit(0);
			}

			// Remove newline from line to compare with delimiter
			if (line[ft_strlen(line) - 1] == '\n')
				line[ft_strlen(line) - 1] = '\0';

			if (ft_strcmp(line, cmd->heredoc_delimeter) == 0)
			{
				free(line);
				close(pipe_fds[WRITE_END]);
				exit(0);
			}

			// Add newline back for writing to pipe
			write(pipe_fds[WRITE_END], line, ft_strlen(line));
			write(pipe_fds[WRITE_END], "\n", 1);
			free(line);
		}
	}
	else
	{
		setup_parent_heredoc_signals();
		close(pipe_fds[WRITE_END]);
		waitpid(pid, &status, 0);
		restore_heredoc_signals();

		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			close(pipe_fds[READ_END]);
			return (130);
		}
		if (WIFSIGNALED(status))
		{
			close(pipe_fds[READ_END]);
			return (1);
		}
		*heredoc_fd = pipe_fds[READ_END];
		return (0);
	}
}

static int	setup_heredoc(t_cmds *cmd)
{
	int		heredoc_fd;
	int		status;

	status = create_heredoc_pipe(cmd, &heredoc_fd);
	if (status != 0)
		return (status);

	// Set up stdin redirection from the heredoc pipe
	if (dup2(heredoc_fd, STDIN_FILENO) < 0)
	{
		close(heredoc_fd);
		return (1);
	}
	close(heredoc_fd);
	return (0);
}


int	setup_redirections(t_cmds *cmd)
{
	int	stdin_backup;
	int	stdout_backup;
	int	status;

	if (!cmd || (!cmd->input_file && !cmd->output_file && !cmd->rw_file && !cmd->heredoc_delimeter && cmd->heredoc_fd < 0))
		return (0);

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);

	status = 0;
	// Use the stored heredoc file descriptor if available
	if (cmd->heredoc_fd >= 0)
	{
		if (dup2(cmd->heredoc_fd, STDIN_FILENO) < 0)
		{
			close(cmd->heredoc_fd);
			status = 1;
		}
		else
		{
			close(cmd->heredoc_fd);
			cmd->heredoc_fd = -1; // Mark as used
		}
	}
	else if (cmd->heredoc_list)
		status = process_all_heredocs(cmd);
	else if (cmd->heredoc_delimeter)
		status = setup_heredoc(cmd);
	// If we have a regular input file after heredoc, it should override the heredoc
	if (status == 0 && cmd->input_file)
		status = setup_input_redirection(cmd);
	if (status == 0 && cmd->output_file)
		status = setup_output_redirection(cmd);
	if (status == 0 && cmd->rw_file)
		status = setup_rw_redirection(cmd);

	if (status != 0)
	{
		reset_redirections(stdin_backup, stdout_backup);
		return (status);
	}
	close(stdin_backup);
	close(stdout_backup);

	return (0);
}

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

int	process_heredoc_for_pipeline(t_cmds *cmd, int *heredoc_fd)
{
	if (!cmd->heredoc_delimeter)
	{
		*heredoc_fd = -1;
		return (0);
	}
	return (create_heredoc_pipe(cmd, heredoc_fd));
}

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

int	process_heredocs_after_parsing(t_cmds *commands)
{
	t_cmds	*current;

	current = commands;
	while (current)
	{
		if (current->heredoc_list)
		{
			// For multiple heredocs, we need to modify process_all_heredocs to return the fd
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
