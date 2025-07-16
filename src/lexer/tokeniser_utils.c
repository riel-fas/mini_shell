/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokeniser_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:13:29 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:13:37 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

t_token	*new_token_set(t_token *token)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = token->type;
	new_token->value = ft_strdup(token->value);
	new_token->next = NULL;
	new_token->prev = NULL;
	new_token->addspace = token->addspace;
	new_token->variable = false;
	return (new_token);
}

t_token	*dollar_sign_joining(t_token *token, t_token *new_token)
{
	char	*prefix;
	char	*value;

	if (ft_strlen(token->value) == 1)
		return (free_token(token), new_token);
	else
	{
		prefix = ft_substr(token->value, 0, ft_strlen(token->value) - 1);
		value = ft_strjoin(prefix, new_token->value);
		free(prefix);
		free_token(token);
		free(new_token->value);
		new_token->value = value;
		return (new_token);
	}
}
