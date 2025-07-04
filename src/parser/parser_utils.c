/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 00:56:46 by roubelka          #+#    #+#             */
/*   Updated: 2025/07/04 19:40:53 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"

void	free_heredoc(t_heredoc_list *heredoc_list)
{
	t_heredoc_list	*temp;

	while (heredoc_list)
	{
		temp = heredoc_list;
		heredoc_list = heredoc_list->next;
		free(temp->delimiter);
		free(temp);
	}
}

static void	free_single_command(t_cmds *cmd)
{
	int	i;

	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	free(cmd->input_file);
	free(cmd->output_file);
	free(cmd->rw_file);
	free(cmd->heredoc_delimeter);
	if (cmd->heredoc_fd >= 0)
		close(cmd->heredoc_fd);
	free_heredoc(cmd->heredoc_list);
	free(cmd);
}

void	free_commands(t_cmds *cmds)
{
	t_cmds	*next;

	while (cmds)
	{
		next = cmds->next;
		free_single_command(cmds);
		cmds = next;
	}
}
