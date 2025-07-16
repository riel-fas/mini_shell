/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 23:56:33 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/15 23:56:41 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

void	free_cmd(t_cmd *cmdarg_list)
{
	t_cmd	*tmp;

	while (cmdarg_list)
	{
		tmp = cmdarg_list;
		cmdarg_list = cmdarg_list->next;
		free_node(tmp);
	}
}

void	free_token_list(t_token *token_list)
{
	t_token	*tmp;

	while (token_list)
	{
		tmp = token_list;
		token_list = token_list->next;
		free(tmp->value);
		free(tmp);
	}
}

void	free_token(t_token *token)
{
	if (token)
	{
		free(token->value);
		free(token);
	}
}

void	free_node(t_cmd *node)
{
	int	i;

	i = 0;
	if (node)
	{
		if (node->cmd)
		{
			while (i < node->cmdsize)
				free(node->cmd[i++]);
			free(node->cmd);
		}
		free_redirection(node->redirections);
		free(node);
	}
}

void	free_redirection(t_redi *redi)
{
	t_redi	*tmp;

	while (redi)
	{
		tmp = redi;
		redi = redi->next;
		free(tmp);
	}
}
