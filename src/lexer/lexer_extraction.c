/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 19:15:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/05/22 16:43:01 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

//Extract a word token from input // katjbed token dyal WORD men strings
char	*extract_word(char *input, int *i)
{
	int		start;
	int		len;
	char	*word;

	start = *i;
	len = 0;
	while (input[*i] && !is_whitespace(input[*i]) &&
		   !is_operator(input[*i]) && !is_quote(input[*i]))
	{
		(*i)++;
		len++;
	}
	word = ft_substr(input, start, len);
	return (word);
}

//Extract an operator token from input // katjbed OPEARATERS tokens
char	*extract_operator(char *input, int *i, t_token_type *type)
{
	char	*operator;

	if (input[*i] == '>' && input[*i + 1] == '>')
	{
		operator = ft_strdup(">>");
		*type = TOKEN_REDIR_APPEND;
		(*i) += 2;
	}
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		operator = ft_strdup("<<");
		*type = TOKEN_HEREDOC;
		(*i) += 2;
	}
	else if (input[*i] == '>')
	{
		operator = ft_strdup(">");
		*type = TOKEN_REDIR_OUT;
		(*i)++;
	}
	else if (input[*i] == '<')
	{
		operator = ft_strdup("<");
		*type = TOKEN_REDIR_IN;
		(*i)++;
	}
	else if (input[*i] == '|')
	{
		operator = ft_strdup("|");
		*type = TOKEN_PIPE;
		(*i)++;
	}
	else
	{
		operator = NULL;
		*type = TOKEN_WORD;
	}
	return (operator);
}

//Extract a quoted string from input // kanhdliw biha string li ma bin ""
char	*extract_quoted_string(char *input, int *i, char quote)
{
	int		start;
	int		len;
	char	*str;

	start = *i;
	len = 0;

	while (input[*i] && input[*i] != quote)
	{
		(*i)++;
		len++;
	}
	if (input[*i] != quote)
		return (NULL); // Unclosed quote
	// Extract the quoted string (without quotes)
	str = ft_substr(input, start, len);
	(*i)++;
	return (str);
}
