/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:08:39 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:11:11 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

static int	cmd_overload(t_cmd **node, t_shell *shell)
{
	if ((*node)->cmdsize >= (*node)->cmd_capacity)
	{
		if (!cmd_array_res(node, (*node)->cmd_capacity * 2 + 1))
		{
			perror("Failed to resize cmd array");
			shell->exit_status = 1;
			return (0);
		}
	}
	return (1);
}

static void	word_check(t_cmd **node, t_token *token_list, char *val)
{
	char	*tmp;

	if (token_list->current->prev != NULL
		&& token_list->current->prev->addspace == true)
		(*node)->cmd[(*node)->cmdsize++] = val;
	else
	{
		if ((*node)->cmdsize > 0)
		{
			tmp = (*node)->cmd[(*node)->cmdsize - 1];
			(*node)->cmd[(*node)->cmdsize - 1] = ft_strjoin(tmp, val);
			free(tmp);
			free(val);
		}
		else
			(*node)->cmd[(*node)->cmdsize++] = val;
	}
}

void	*word_parsing(t_cmd **node, t_token *token_list, t_shell *shell)
{
	char	*val;

	val = ft_strdup(token_list->current->value);
	if (val == NULL)
		return (perror("malloc failure"), shell->exit_status = 1, NULL);
	if (token_list->current->variable == true
		&& token_list->current->type == WORD)
		splited_words_handling(node, token_list, val, shell);
	else
	{
		if (!cmd_overload(node, shell))
		{
			free(val);
			return (NULL);
		}
		word_check(node, token_list, val);
	}
	return (NULL);
}

void	redi_pars(t_cmd **node, t_token *token_list)
{
	if (token_list->current->type == INPUT
		|| token_list->current->type == HEREDOC)
	{
		if (token_list->current->next->type == WORD)
			add_redir(&(*node)->redirections,
				set_new_redir(token_list->current, true));
		else
			add_redir(&(*node)->redirections,
				set_new_redir(token_list->current, false));
	}
	else
		add_redir(&(*node)->redirections,
			set_new_redir(token_list->current, true));
	token_list->current = token_list->current->next;
}

t_cmd	*parsing(t_token *token_list, t_list *minienv, t_shell *shell)
{
	t_cmd	*cmdarg_list;
	t_cmd	*node;
	t_token	*tmp;

	cmdarg_list = NULL;
	tmp = token_list;
	if (!token_list)
		return (NULL);
	while (tmp)
	{
		if (!heredoc_cant_exp(tmp))
			variables_expanding(&tmp, minienv, shell);
		tmp = tmp->next;
	}
	token_list->current = token_list;
	node = next_node(token_list, shell);
	while (node)
	{
		if (node)
			add_node_back(&cmdarg_list, create_new_node(node));
		freeall(node->cmd, node->cmdsize);
		free(node);
		node = next_node(token_list, shell);
	}
	return (cmdarg_list);
}
