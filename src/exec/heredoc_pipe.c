/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 02:50:26 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/04 02:50:26 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

void	process_heredoc_child(int write_fd, char *delimiter)
{
	char	*line;

	setup_heredoc_signals();
	while (1)
	{
		ft_putstr_fd("> ", STDERR_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line)
		{
			ft_putchar_fd('\n', STDERR_FILENO);
			break ;
		}
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, write_fd);
		ft_putchar_fd('\n', write_fd);
		free(line);
	}
	close(write_fd);
	exit(0);
}

int	handle_heredoc_parent(pid_t pid, int read_fd)
{
	int	status;

	setup_parent_heredoc_signals();
	waitpid(pid, &status, 0);
	restore_heredoc_signals();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(read_fd);
		g_state.exit_status = 130;
		return (130);
	}
	return (0);
}

static int	init_heredoc_pipe(int pipe_fds[2])
{
	if (pipe(pipe_fds) < 0)
		return (1);
	return (0);
}

static pid_t	fork_heredoc_process(int pipe_fds[2])
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		close(pipe_fds[READ_END]);
		close(pipe_fds[WRITE_END]);
		return (-1);
	}
	return (pid);
}

int	create_heredoc_pipe(t_cmds *cmd, int *heredoc_fd)
{
	int		pipe_fds[2];
	pid_t	pid;
	int		status;

	if (!cmd->heredoc_delimeter)
		return (0);
	if (init_heredoc_pipe(pipe_fds) != 0)
		return (1);
	pid = fork_heredoc_process(pipe_fds);
	if (pid == -1)
		return (1);
	if (pid == 0)
		process_heredoc_child(pipe_fds[WRITE_END], cmd->heredoc_delimeter);
	close(pipe_fds[WRITE_END]);
	status = handle_heredoc_parent(pid, pipe_fds[READ_END]);
	if (status != 0)
		return (status);
	*heredoc_fd = pipe_fds[READ_END];
	return (0);
}
