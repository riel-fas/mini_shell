/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_extraction_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 19:15:00 by codespace        #+#    #+#             */
/*   Updated: 2025/07/04 00:00:00 by codespace       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"

/* Memory and buffer management */
static char	*handle_realloc(char **result, size_t *capacity)
{
	char	*new_result;
	size_t	old_capacity;

	old_capacity = *capacity;
	*capacity *= 2;
	new_result = ft_realloc(*result, old_capacity, *capacity);
	if (!new_result)
	{
		free(*result);
		return (NULL);
	}
	return (new_result);
}

int	add_char_to_buffer(char **result, size_t *capacity, char c)
{
	int	len;

	len = ft_strlen(*result);
	if (len >= (int)(*capacity - 1))
	{
		*result = handle_realloc(result, capacity);
		if (!*result)
			return (0);
	}
	(*result)[len++] = c;
	(*result)[len] = '\0';
	return (1);
}

/* Operator context management */
void	set_operator_context(int *i, int inc, t_token_type *type,
			t_token_type token_type)
{
	*i += inc;
	*type = token_type;
}

/* Operator detection */
int	is_special_operator(char *input, int i)
{
	return ((input[i] == '>' && input[i + 1] == '>')
		|| (input[i] == '<' && input[i + 1] == '<')
		|| (input[i] == '<' && input[i + 1] == '>'));
}
