/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 04:30:46 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 04:30:58 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ';');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

int	all_tokens_empty(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (1);
	current = tokens;
	while (current)
	{
		if (current->value && current->value[0] != '\0')
			return (0);
		current = current->next;
	}
	return (1);
}
