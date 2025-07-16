/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 23:57:05 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/15 23:57:14 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

t_cmd	*create_new_node(t_cmd *node)
{
	t_cmd		*new;
	int			i;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->cmdsize = node->cmdsize;
	new->cmd_capacity = node->cmd_capacity;
	new->cmd = malloc(sizeof(char *) * (node->cmd_capacity + 1));
	if (!new->cmd)
		return (free(new), NULL);
	i = -1;
	while (++i < node->cmdsize)
	{
		if (node->cmd[i])
			new->cmd[i] = ft_strdup(node->cmd[i]);
		else
			new->cmd[i] = NULL;
	}
	new->cmd[i] = NULL;
	new->is_builtin = node->is_builtin;
	new->redirections = node->redirections;
	new->input = node->input;
	new->next = NULL;
	return (new);
}

void	add_node_back(t_cmd **lst, t_cmd *new)
{
	t_cmd	*tmp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_cmd	*node_init(void)
{
	t_cmd	*node;

	node = malloc(sizeof(t_cmd));
	if (!node)
		return (NULL);
	node->input = NULL;
	node->redirections = NULL;
	node->is_builtin = false;
	node->next = NULL;
	node->output = NULL;
	node->cmd = NULL;
	node->cmdsize = 0;
	node->cmd_capacity = 0;
	return (node);
}

t_cmd	*next_node(t_token *token_list, t_shell *shell)
{
	t_cmd		*node;
	int			initial_capacity;

	if (!token_list || !token_list->current)
		return (NULL);
	node = node_init();
	initial_capacity = toksizer(token_list);
	node->cmd = malloc(sizeof(char *) * (initial_capacity + 1));
	if (!node->cmd)
		return (free(node), NULL);
	node->cmd_capacity = initial_capacity;
	if (token_list->current->type == PIPE)
		token_list->current = token_list->current->next;
	if (!token_list->current)
		return (free(node->cmd), free(node), NULL);
	while (token_list->current && token_list->current->type != PIPE)
	{
		if (token_list->current && check_if_cmd(token_list->current))
			word_parsing(&node, token_list, shell);
		else if (token_list->current && rediction_rec(token_list->current))
			redi_pars(&node, token_list);
		token_list->current = token_list->current->next;
	}
	node->cmd[node->cmdsize] = NULL;
	return (node);
}

bool	cmd_array_res(t_cmd **node, int new_capacity)
{
	char	**new_cmd;
	int		i;

	if (new_capacity <= (*node)->cmd_capacity)
		return (true);
	new_cmd = malloc(sizeof(char *) * (new_capacity + 1));
	if (!new_cmd)
		return (false);
	i = 0;
	while (i < (*node)->cmdsize)
	{
		new_cmd[i] = (*node)->cmd[i];
		i++;
	}
	while (i <= new_capacity)
	{
		new_cmd[i] = NULL;
		i++;
	}
	free((*node)->cmd);
	(*node)->cmd = new_cmd;
	(*node)->cmd_capacity = new_capacity;
	return (true);
}
