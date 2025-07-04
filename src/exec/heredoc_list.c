/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 02:50:26 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/04 02:50:26 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

static int	handle_heredoc_node(t_cmds *cmd, t_heredoc_list *current)
{
	t_cmds	temp_cmd;
	int		dummy_fd;

	temp_cmd = *cmd;
	temp_cmd.heredoc_delimeter = current->delimiter;
	if (current->next == NULL)
	{
		if (create_heredoc_pipe(&temp_cmd, &cmd->heredoc_fd) != 0)
			return (1);
	}
	else
	{
		if (create_heredoc_pipe(&temp_cmd, &dummy_fd) != 0)
			return (1);
		if (dummy_fd >= 0)
			close(dummy_fd);
	}
	return (0);
}

int	process_all_heredocs(t_cmds *cmd)
{
	t_heredoc_list	*current;

	if (!cmd->heredoc_list)
		return (0);
	current = cmd->heredoc_list;
	while (current)
	{
		if (handle_heredoc_node(cmd, current) != 0)
			return (1);
		current = current->next;
	}
	return (0);
}

int	process_single_heredoc(t_heredoc_list *current, t_cmds *cmd, int is_last)
{
	t_cmds	temp_cmd;
	int		dummy_fd;

	ft_memset(&temp_cmd, 0, sizeof(t_cmds));
	temp_cmd.heredoc_delimeter = current->delimiter;
	if (is_last)
	{
		if (create_heredoc_pipe(&temp_cmd, &cmd->heredoc_fd) != 0)
			return (1);
	}
	else
	{
		if (create_heredoc_pipe(&temp_cmd, &dummy_fd) != 0)
			return (1);
		if (dummy_fd >= 0)
			close(dummy_fd);
	}
	return (0);
}

int	process_heredoc_list(t_cmds *cmd)
{
	t_heredoc_list	*current;

	if (!cmd->heredoc_list)
		return (0);
	current = cmd->heredoc_list;
	while (current)
	{
		if (process_single_heredoc(current, cmd, (current->next == NULL)) != 0)
			return (1);
		current = current->next;
	}
	return (0);
}
