/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 19:15:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/05/22 13:05:21 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/**
 * Extract a word token from input
 *
 * @param input Input string
 * @param i Current position (will be updated)
 * @return Dynamically allocated word or NULL on error
 */
char	*extract_word(char *input, int *i)
{
	int		start;
	int		len;
	char	*word;

	start = *i;
	len = 0;

	// Count word length until whitespace, operator, quote or end of string
	while (input[*i] && !is_whitespace(input[*i]) &&
		   !is_operator(input[*i]) && !is_quote(input[*i]))
	{
		(*i)++;
		len++;
	}

	// Extract the word
	word = ft_substr(input, start, len);

	return (word);
}

/**
 * Extract an operator token from input
 *
 * @param input Input string
 * @param i Current position (will be updated)
 * @param type Output parameter for token type
 * @return Dynamically allocated operator string or NULL on error
 */
char	*extract_operator(char *input, int *i, t_token_type *type)
{
	char	*op;

	// Handle >> (append output)
	if (input[*i] == '>' && input[*i + 1] == '>')
	{
		op = ft_strdup(">>");
		*type = TOKEN_REDIR_APPEND;
		(*i) += 2;
	}
	// Handle << (heredoc)
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		op = ft_strdup("<<");
		*type = TOKEN_HEREDOC;
		(*i) += 2;
	}
	// Handle > (output redirection)
	else if (input[*i] == '>')
	{
		op = ft_strdup(">");
		*type = TOKEN_REDIR_OUT;
		(*i)++;
	}
	// Handle < (input redirection)
	else if (input[*i] == '<')
	{
		op = ft_strdup("<");
		*type = TOKEN_REDIR_IN;
		(*i)++;
	}
	// Handle | (pipe)
	else if (input[*i] == '|')
	{
		op = ft_strdup("|");
		*type = TOKEN_PIPE;
		(*i)++;
	}
	else
	{
		// Should never happen
		op = NULL;
		*type = TOKEN_WORD;
	}

	return (op);
}

/**
 * Extract a quoted string from input
 *
 * @param input Input string
 * @param i Current position (will be updated)
 * @param quote Quote character to match (' or ")
 * @return Dynamically allocated string or NULL if quote is unclosed
 */
char	*extract_quoted_string(char *input, int *i, char quote)
{
	int		start;
	int		len;
	char	*str;

	start = *i;
	len = 0;

	// Find the closing quote
	while (input[*i] && input[*i] != quote)
	{
		(*i)++;
		len++;
	}

	// Check if quote was closed
	if (input[*i] != quote)
		return (NULL); // Unclosed quote

	// Extract the quoted string (without quotes)
	str = ft_substr(input, start, len);

	// Skip the closing quote
	(*i)++;

	return (str);
}
