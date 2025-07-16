/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:05:47 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:06:11 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

bool	rediction_rec(t_token *token)
{
	return (token->type == INPUT || token->type == OUTPUT
		|| token->type == HEREDOC || token->type == APPEND);
}

t_redi	*set_new_redir(t_token *token, bool expand)
{
	t_redi	*new;

	new = malloc(sizeof(t_redi));
	if (!new)
		return (NULL);
	new->type = token->type;
	new->file = token->next->value;
	new->expand = expand;
	new->variable = token->next->variable;
	new->next = NULL;
	return (new);
}

void	add_redir(t_redi **redi, t_redi *new)
{
	t_redi	*tmp;

	if (!*redi)
	{
		*redi = new;
		return ;
	}
	tmp = *redi;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}
