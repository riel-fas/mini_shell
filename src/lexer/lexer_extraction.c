/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_extraction_word.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 19:15:00 by codespace        #+#    #+#             */
/*   Updated: 2025/07/04 00:00:00 by codespace       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"

/* Character processing functions */
static int	process_regular_char(char *input, int *i,
			char **result, size_t *capacity)
{
	if (!add_char_to_buffer(result, capacity, input[*i]))
		return (0);
	(*i)++;
	return (1);
}

static int	process_quote_content(char *input, int *i,
			char **result, size_t *capacity)
{
	char	quote;

	quote = input[*i];
	if (!add_char_to_buffer(result, capacity, quote))
		return (0);
	(*i)++;
	while (input[*i] && input[*i] != quote)
	{
		if (!add_char_to_buffer(result, capacity, input[*i]))
			return (0);
		(*i)++;
	}
	if (input[*i] == quote)
	{
		if (!add_char_to_buffer(result, capacity, input[*i]))
			return (0);
		(*i)++;
	}
	return (1);
}

/* Main word extraction */
char	*extract_word(char *input, int *i)
{
	char		*result;
	size_t		capacity;
	char		*final_result;

	capacity = 256;
	result = malloc(capacity);
	if (!result)
		return (NULL);
	result[0] = '\0';
	while (input[*i] && !is_whitespace(input[*i]) && !is_operator(input[*i]))
	{
		if (is_quote(input[*i])
			&& !process_quote_content(input, i, &result, &capacity))
			break ;
		else if (!process_regular_char(input, i, &result, &capacity))
			break ;
	}
	final_result = ft_strdup(result);
	free(result);
	return (final_result);
}

/* Quoted string extraction */
char	*extract_quoted_string(char *input, int *i, char quote)
{
	int		start;
	int		len;
	char	*str;

	(*i)++;
	start = *i;
	len = 0;
	while (input[*i] && input[*i] != quote)
	{
		(*i)++;
		len++;
	}
	if (input[*i] != quote)
		return (NULL);
	if (len == 0)
		str = ft_strdup("");
	else
		str = ft_substr(input, start, len);
	(*i)++;
	return (str);
}
