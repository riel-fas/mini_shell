/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:26:09 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/29 17:42:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"

static t_token	*handle_token_creation(char *value, t_token_type type,
				t_token *tokens)
{
	t_token	*new_token;

	if (!value)
	{
		free_tokens(tokens);
		return (NULL);
	}
	new_token = create_token(value, type);
	free(value);
	if (!new_token)
	{
		free_tokens(tokens);
		return (NULL);
	}
	return (new_token);
}

static char	*get_token_value(char *input, int *i, t_token_type *type)
{
	if (is_operator(input[*i]))
		return (extract_operator(input, i, type));
	else
	{
		*type = TOKEN_WORD;
		return (extract_word(input, i));
	}
}

t_token	*tokenize(char *input)
{
	t_token			*tokens;
	t_token			*new_token;
	int				i;
	char			*value;
	t_token_type	type;

	if (!input)
		return (NULL);
	tokens = NULL;
	i = 0;
	while (input[i])
	{
		int old_i = i;  // Save position for safety check
		while (is_whitespace(input[i]))
			i++;
		if (input[i] == '\0')
			break;
		value = get_token_value(input, &i, &type);
		new_token = handle_token_creation(value, type, tokens);
		if (!new_token)
			return (NULL);
		add_token(&tokens, new_token);
		if (i <= old_i)
			i = old_i + 1;
	}
	return (tokens);
}
