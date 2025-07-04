/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 21:19:57 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 21:20:03 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

int	handle_heredoc_redirection(t_cmds *cmd)
{
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
	return (0);
}

int	handle_rw_redirection(t_cmds *cmd)
{
	int	fd;

	if (!cmd->rw_file)
		return (0);
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
	return (0);
}

int	handle_exec_redirections(t_cmds *cmd)
{
	if (!cmd)
		return (0);
	if (handle_heredoc_redirection(cmd) != 0)
		return (1);
	if (handle_input_redirection(cmd) != 0)
		return (1);
	if (handle_output_redirection(cmd) != 0)
		return (1);
	if (handle_rw_redirection(cmd) != 0)
		return (1);
	return (0);
}

static int	needs_redirections(t_cmds *cmd)
{
	if (cmd->input_file || cmd->output_file || cmd->rw_file)
		return (1);
	if (cmd->heredoc_delimeter || cmd->heredoc_fd >= 0)
		return (1);
	if (cmd->heredoc_list)
		return (1);
	return (0);
}

int	setup_redirections(t_cmds *cmd)
{
	int	stdin_backup;
	int	stdout_backup;
	int	status;

	if (!cmd || !needs_redirections(cmd))
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
