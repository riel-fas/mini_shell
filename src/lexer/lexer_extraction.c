/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 19:15:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/30 13:55:48 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"


char	*extract_word(char *input, int *i)
{
	int		start;
	int		len;
	char	*word;
	char	quote;

	start = *i;
	len = 0;

	while (input[*i] && !is_whitespace(input[*i]) && !is_operator(input[*i]))
	{
		// Handle special $' and $" quoting
		if (input[*i] == '$' && (input[*i + 1] == '\'' || input[*i + 1] == '"'))
		{
			(*i)++; // Skip the $
			len++;
			quote = input[*i]; // Get the quote type (' or ")
			(*i)++; // Skip opening quote
			len++;
			// Include everything until closing quote
			while (input[*i] && input[*i] != quote)
			{
				(*i)++;
				len++;
			}
			if (input[*i] == quote)
			{
				(*i)++; // Skip closing quote
				len++;
			}
		}
		else if (is_quote(input[*i]))
		{
			quote = input[*i];
			(*i)++; // Skip opening quote
			len++;
			// Include everything until closing quote
			while (input[*i] && input[*i] != quote)
			{
				(*i)++;
				len++;
			}
			if (input[*i] == quote)
			{
				(*i)++; // Skip closing quote
				len++;
			}
		}
		else
		{
			(*i)++;
			len++;
		}
	}
	word = ft_substr(input, start, len);
	return (word);
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

