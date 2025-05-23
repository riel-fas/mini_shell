/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:26:09 by riel-fas          #+#    #+#             */
/*   Updated: 2025/05/23 22:21:59 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

//Tokenize input string into a list of tokens // kanredo input 3la chkel list dyal tokens
// t_token	*tokenize(char *input)
// {
// 	t_token			*tokens;
// 	t_token			*new_token;
// 	int				i;
// 	char			*value;
// 	t_token_type	type;

// 	if (!input)
// 		return (NULL);
// 	tokens = NULL;
// 	i = 0;
// 	while (input[i])
// 	{
// 		while (is_whitespace(input[i]))
// 			i++;
// 		if (input[i] == '\0')
// 			break;
// 		if (is_operator(input[i]))
// 		{
// 			value = extract_operator(input, &i, &type);
// 			if (!value)
// 			{
// 				free_tokens(tokens);
// 				return (NULL);
// 			}
// 			new_token = create_token(value, type);
// 			free(value);
// 		}
// 		else if (is_quote(input[i]))
// 		{
// 			char quote = input[i++]; // Save quote type and move past it
// 			value = extract_quoted_string(input, &i, quote);
// 			if (!value)
// 			{
// 				free_tokens(tokens);
// 				return (NULL);
// 			}
// 			new_token = create_token(value, TOKEN_WORD);
// 			free(value);
// 		}
// 		else
// 		{
// 			value = extract_word(input, &i);
// 			if (!value)
// 			{
// 				free_tokens(tokens);
// 				return (NULL);
// 			}
// 			new_token = create_token(value, TOKEN_WORD);
// 			free(value);
// 		}
// 		if (!new_token)
// 		{
// 			free_tokens(tokens);
// 			return (NULL);
// 		}
// 		add_token(&tokens, new_token);
// 	}
// 	return (tokens);
// }


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
	else if (is_quote(input[*i]))
	{
		char quote = input[(*i)++];
		*type = TOKEN_WORD;
		return (extract_quoted_string(input, i, quote));
	}
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
		while (is_whitespace(input[i]))
			i++;
		if (input[i] == '\0')
			break;
		value = get_token_value(input, &i, &type);
		new_token = handle_token_creation(value, type, tokens);
		if (!new_token)
			return (NULL);
		add_token(&tokens, new_token);
	}
	return (tokens);
}

//Debug function to print all tokens // hadi function khir bach ndebugi biha
// hadi khir bach ndebagiw l tokens
void	print_tokens(t_token *tokens)
{
	t_token	*current;
	int		i;

	if (!tokens)
	{
		printf("No tokens to print\n");
		return;
	}

	current = tokens;
	i = 0;

	printf("\n--- Tokens ---\n");
	while (current)
	{
		printf("Token %d: Type = ", i);

		// Print token type as string
		switch (current->type)
		{
			case TOKEN_WORD:
				printf("WORD");
				break;
			case TOKEN_PIPE:
				printf("PIPE");
				break;
			case TOKEN_REDIR_IN:
				printf("REDIR_IN");
				break;
			case TOKEN_REDIR_OUT:
				printf("REDIR_OUT");
				break;
			case TOKEN_REDIR_APPEND:
				printf("REDIR_APPEND");
				break;
			case TOKEN_HEREDOC:
				printf("HEREDOC");
				break;
			case TOKEN_END_OF_INPUT:
				printf("END_OF_INPUT");
				break;
			default:
				printf("UNKNOWN");
		}

		printf(", Value = \"%s\"\n", current->value);

		current = current->next;
		i++;
	}
	printf("--------------\n");
}
