/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 05:11:41 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 05:12:18 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"

/*
 * Helper: Extracts special variable names like ?, $, or digits
 */
static char	*extract_special_var_name(char *input, int *i)
{
	char	digit_str[2];

	if (input[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	if (input[*i] == '$')
	{
		(*i)++;
		return (ft_strdup("$"));
	}
	if (ft_isdigit(input[*i]))
	{
		digit_str[0] = input[*i];
		digit_str[1] = '\0';
		(*i)++;
		return (ft_strdup(digit_str));
	}
	return (NULL);
}

/*
 * Extracts a variable name after $
 * Handles special cases like $?, $$, $1, etc.
 */
char	*extract_var_name(char *input, int *i)
{
	int		start;
	int		len;
	char	*special;

	special = extract_special_var_name(input, i);
	if (special)
		return (special);
	start = *i;
	len = 0;
	if (ft_isalpha(input[*i]) || input[*i] == '_')
	{
		while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		{
			(*i)++;
			len++;
		}
		return (ft_substr(input, start, len));
	}
	return (NULL);
}
