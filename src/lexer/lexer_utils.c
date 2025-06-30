/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 19:15:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/30 18:25:06 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"

t_token	*create_token(char *value, t_token_type type)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->original_value = ft_strdup(value);
	if (!token->original_value)
	{
		free(token->value);
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **tokens, t_token *new_token)
{
	t_token	*current;

	if (!tokens || !new_token)
		return;
	if (!*tokens)
	{
		*tokens = new_token;
		return;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*temp;

	current = tokens;
	while (current)
	{
		temp = current;
		current = current->next;
		free(temp->value);
		free(temp->original_value);
		free(temp);
	}
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ';');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

int	all_tokens_empty(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (1);
	current = tokens;
	while (current)
	{
		if (current->value && current->value[0] != '\0')
			return (0);
		current = current->next;
	}
	return (1);
}
