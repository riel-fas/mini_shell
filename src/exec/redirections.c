/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:45:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/18 22:34:04 by riel-fas         ###   ########.fr       */
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

static int	setup_heredoc(t_cmds *cmd)
{
	int		pipe_fds[2];
	char	*line;
	int		status;

	if (!cmd->heredoc_delimeter)
		return (0);

	if (pipe(pipe_fds) < 0)
		return (1);

	printf("heredoc> "); // Simple prompt
	line = readline("");
	while (line && ft_strcmp(line, cmd->heredoc_delimeter) != 0)
	{
		ft_putendl_fd(line, pipe_fds[WRITE_END]);
		free(line);
		printf("heredoc> ");
		line = readline("");
	}
	if (line)
		free(line);

	close(pipe_fds[WRITE_END]);
	status = dup2(pipe_fds[READ_END], STDIN_FILENO);
	close(pipe_fds[READ_END]);
	return (status < 0);
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

	// Only close backups if there was an error
	// Otherwise, the caller is responsible for resetting redirections
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
