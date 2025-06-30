/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 19:15:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/30 20:18:37 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"


char	*extract_word(char *input, int *i)
{
	char	*result;
	int		capacity;
	int		len;
	char	quote;

	capacity = 256;  // Initial capacity
	result = malloc(capacity);
	if (!result)
		return (NULL);
	len = 0;

	while (input[*i] && !is_whitespace(input[*i]) && !is_operator(input[*i]))
	{
		// Reallocate if needed
		if (len >= capacity - 1)
		{
			capacity *= 2;
			char *new_result = realloc(result, capacity);
			if (!new_result)
			{
				free(result);
				return (NULL);
			}
			result = new_result;
		}

		if (is_quote(input[*i]))
		{
			quote = input[*i];
			(*i)++; // Skip opening quote
			result[len++] = quote;  // Include the quote in result
			// Include everything until closing quote
			while (input[*i] && input[*i] != quote)
			{
				if (len >= capacity - 1)
				{
					capacity *= 2;
					char *new_result = realloc(result, capacity);
					if (!new_result)
					{
						free(result);
						return (NULL);
					}
					result = new_result;
				}
				result[len++] = input[(*i)++];
			}
			if (input[*i] == quote)
			{
				result[len++] = input[(*i)++]; // Include closing quote
			}
		}
		else if (input[*i] == '$' && (input[*i + 1] == '\'' || input[*i + 1] == '"'))
		{
			// Handle special $' and $" quoting - skip the $ completely
			(*i)++; // Skip the $
			quote = input[*i]; // Get the quote type (' or ")
			(*i)++; // Skip opening quote
			result[len++] = quote;  // Include opening quote (without $)
			// Include everything until closing quote
			while (input[*i] && input[*i] != quote)
			{
				if (len >= capacity - 1)
				{
					capacity *= 2;
					char *new_result = realloc(result, capacity);
					if (!new_result)
					{
						free(result);
						return (NULL);
					}
					result = new_result;
				}
				result[len++] = input[(*i)++];
			}
			if (input[*i] == quote)
			{
				result[len++] = input[(*i)++]; // Include closing quote
			}
		}
		else
		{
			result[len++] = input[(*i)++];
		}
	}
	result[len] = '\0';

	// Create final result with exact size
	char *final_result = ft_strdup(result);
	free(result);
	return (final_result);
}


static char	*set_operator(char *op, t_token_type token_type, int *i, int increment, t_token_type *type)
{
	*i += increment;
	*type = token_type;
	return (ft_strdup(op));
}

char	*extract_operator(char *input, int *i, t_token_type *type)
{
	if (input[*i] == '>' && input[*i + 1] == '>')
		return (set_operator(">>", TOKEN_REDIR_APPEND, i, 2, type));
	else if (input[*i] == '<' && input[*i + 1] == '<')
		return (set_operator("<<", TOKEN_HEREDOC, i, 2, type));
	else if (input[*i] == '<' && input[*i + 1] == '>')
		return (set_operator("<>", TOKEN_REDIR_READ_WRITE, i, 2, type));
	else if (input[*i] == '>')
		return (set_operator(">", TOKEN_REDIR_OUT, i, 1, type));
	else if (input[*i] == '<')
		return (set_operator("<", TOKEN_REDIR_IN, i, 1, type));
	else if (input[*i] == '|')
		return (set_operator("|", TOKEN_PIPE, i, 1, type));
	else if (input[*i] == ';')
		return (set_operator(";", TOKEN_SEMICOLON, i, 1, type));
	*type = TOKEN_WORD;
	return (NULL);
}


char	*extract_quoted_string(char *input, int *i, char quote)
{
	int		start;
	int		len;
	char	*str;

	(*i)++; // Skip opening quote
	start = *i;
	len = 0;
	while (input[*i] && input[*i] != quote)
	{
		(*i)++;
		len++;
	}
	if (input[*i] != quote)
		return (NULL); // Unclosed quote error
	if (len == 0)
		str = ft_strdup("");
	else
		str = ft_substr(input, start, len);
	(*i)++; // Skip closing quote
	return (str);
}

