/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 12:38:00 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 22:16:12 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"
#include "../../includes/memory.h"

void	free_str_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
}

void	free_token_node(t_token *token)
{
	if (!token)
		return ;
	if (token->value)
		free(token->value);
	if (token->original_value)
		free(token->original_value);
	free(token);
}

void	free_token_list(t_token *tokens)
{
	t_token	*tmp;
	t_token	*next;

	tmp = tokens;
	while (tmp)
	{
		next = tmp->next;
		free_token_node(tmp);
		tmp = next;
	}
}

void	free_heredoc_list(t_heredoc_list *heredoc_list)
{
	t_heredoc_list	*tmp;
	t_heredoc_list	*next;

	tmp = heredoc_list;
	while (tmp)
	{
		next = tmp->next;
		if (tmp->delimiter)
			free(tmp->delimiter);
		free(tmp);
		tmp = next;
	}
}

void	free_command_node(t_cmds *cmd)
{
	if (!cmd)
		return ;
	if (cmd->args)
		free_str_array(cmd->args);
	if (cmd->input_file)
		free(cmd->input_file);
	if (cmd->output_file)
		free(cmd->output_file);
	if (cmd->rw_file)
		free(cmd->rw_file);
	if (cmd->heredoc_delimeter)
		free(cmd->heredoc_delimeter);
	if (cmd->heredoc_list)
		free_heredoc_list(cmd->heredoc_list);
	if (cmd->heredoc_fd > 2)
		close(cmd->heredoc_fd);
	free(cmd);
}
