/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 03:38:07 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 03:47:11 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

int	process_command_heredocs(t_cmds *current)
{
	if (current->heredoc_list)
	{
		if (process_all_heredocs(current) != 0)
			return (1);
	}
	else if (current->heredoc_delimeter)
	{
		if (create_heredoc_pipe(current, &current->heredoc_fd) != 0)
			return (1);
	}
	return (0);
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
