/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:45:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/02 18:15:00 by codespace        ###   ########.fr       */
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
	int		tty_fd;

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

		// Open /dev/tty directly to avoid stdin issues
		tty_fd = open("/dev/tty", O_RDONLY);
		if (tty_fd < 0)
		{
			close(pipe_fds[WRITE_END]);
			exit(1);
		}

		signal(SIGINT, SIG_DFL);
		while (1)
		{
			write(STDOUT_FILENO, "> ", 3);
			fflush(stdout);

			// Read directly from terminal
			line = NULL;
			char buffer[1024];
			int len = 0;
			char c;
			int bytes_read;

			while ((bytes_read = read(tty_fd, &c, 1)) > 0 && c != '\n')
			{
				if (len < 1023)
					buffer[len++] = c;
			}

			// Handle EOF (Ctrl+D)
			if (bytes_read <= 0 && len == 0)
			{
				close(tty_fd);
				close(pipe_fds[WRITE_END]);
				exit(0);
			}

			buffer[len] = '\0';
			line = ft_strdup(buffer);

			if (ft_strcmp(line, cmd->heredoc_delimeter) == 0)
			{
				free(line);
				close(tty_fd);
				close(pipe_fds[WRITE_END]);
				exit(0);
			}
			ft_putendl_fd(line, pipe_fds[WRITE_END]);
			free(line);
		}
	}
	else
	{
		close(pipe_fds[WRITE_END]);
		waitpid(pid, &status, 0);

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

	// Debug: Always just consume and close for standalone heredoc
	// Don't redirect stdin for standalone heredoc
	close(heredoc_fd);
	return (0);
}


int	setup_redirections(t_cmds *cmd)
{
	int	stdin_backup;
	int	stdout_backup;
	int	status;

	if (!cmd || (!cmd->input_file && !cmd->output_file && !cmd->rw_file && !cmd->heredoc_delimeter))
		return (0);

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);

	status = 0;
	if (cmd->heredoc_delimeter)
		status = setup_heredoc(cmd);
	if (status == 0 && cmd->rw_file)
		status = setup_rw_redirection(cmd);
	else if (status == 0 && cmd->input_file)
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
