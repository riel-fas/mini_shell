/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:26:09 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/04 04:29:47 by codespace        ###   ########.fr       */
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
	*type = TOKEN_WORD;
	return (extract_word(input, i));
}

static int	tokenize_loop(char *input, t_token **tokens)
{
	int				i;
	int				old_i;
	char			*value;
	t_token			*new_token;
	t_token_type	type;

	i = 0;
	while (input[i])
	{
		old_i = i;
		while (is_whitespace(input[i]))
			i++;
		if (input[i] == '\0')
			break ;
		value = get_token_value(input, &i, &type);
		new_token = handle_token_creation(value, type, *tokens);
		if (!new_token)
			return (1);
		add_token(tokens, new_token);
		if (i <= old_i)
			i = old_i + 1;
	}
	return (0);
}

t_token	*tokenize(char *input)
{
	t_token	*tokens;

	if (!input)
		return (NULL);
	tokens = NULL;
	if (tokenize_loop(input, &tokens))
		return (NULL);
	return (tokens);
}
