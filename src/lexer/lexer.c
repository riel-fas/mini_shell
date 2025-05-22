/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:26:09 by riel-fas          #+#    #+#             */
/*   Updated: 2025/05/22 12:42:26 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/**
 * Tokenize input string into a list of tokens
 *
 * @param input String to tokenize
 * @return Linked list of tokens or NULL on error
 */
t_token	*tokenize(char *input)
{
	t_token		*tokens;
	t_token		*new_token;
	int			i;
	char		*value;
	t_token_type	type;

	if (!input)
		return (NULL);

	tokens = NULL;
	i = 0;

	while (input[i])
	{
		// Skip whitespace
		while (is_whitespace(input[i]))
			i++;

		// End of input
		if (input[i] == '\0')
			break;

		// Handle operators (<, >, |, etc)
		if (is_operator(input[i]))
		{
			value = extract_operator(input, &i, &type);
			if (!value)
			{
				// Error handling - couldn't extract operator
				free_tokens(tokens);
				return (NULL);
			}

			new_token = create_token(value, type);
			free(value);
		}
		// Handle quotes
		else if (is_quote(input[i]))
		{
			char quote = input[i++]; // Save quote type and move past it
			value = extract_quoted_string(input, &i, quote);
			if (!value)
			{
				// Handle unclosed quote error
				free_tokens(tokens);
				return (NULL);
			}

			new_token = create_token(value, TOKEN_WORD);
			free(value);
		}
		// Handle normal words
		else
		{
			value = extract_word(input, &i);
			if (!value)
			{
				// Error handling - couldn't extract word
				free_tokens(tokens);
				return (NULL);
			}

			new_token = create_token(value, TOKEN_WORD);
			free(value);
		}

		// Add token to list
		if (!new_token)
		{
			free_tokens(tokens);
			return (NULL);
		}

		add_token(&tokens, new_token);
	}

	return (tokens);
}

/**
 * Debug function to print all tokens
 *
 * @param tokens Linked list of tokens to print
 */
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
