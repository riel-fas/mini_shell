/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 05:35:11 by roubelka          #+#    #+#             */
/*   Updated: 2025/07/04 18:30:37 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"

static int	syntax_error_pipe_first(t_token *tokens)
{
	if (tokens->next && tokens->next->type == TOKEN_PIPE)
		printf("Syntax error near unexpected token `||`\n");
	else
		printf("Syntax error near unexpected token `|`\n");
	return (0);
}

int	check_pipe_syntax(t_token *tokens)
{
	if (!tokens)
		return (1);
	if (tokens->type == TOKEN_PIPE)
		return (syntax_error_pipe_first(tokens));
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE)
		{
			if (!tokens->next)
			{
				printf("Syntax error near unexpected token `newline`\n");
				return (0);
			}
			else if (tokens->next->type == TOKEN_PIPE)
			{
				printf("Syntax error near unexpected token `||`\n");
				return (0);
			}
		}
		tokens = tokens->next;
	}
	return (1);
}

int	check_standalone_operators(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD
			&& ft_strcmp(current->value, "!") == 0)
		{
			if (!current->next || current->next->type == TOKEN_PIPE)
				return (0);
		}
		current = current->next;
	}
	return (1);
}

static int	syntax_error_and_token(t_token *current)
{
	if (ft_strcmp(current->value, "&&") == 0
		|| (ft_strlen(current->value) > 1
			&& current->value[0] == '&'
			&& current->value[1] == '&'))
	{
		printf("Syntax error near unexpected token `&&`\n");
		return (0);
	}
	else if (current->value[0] == '&' && ft_strlen(current->value) > 2)
	{
		printf("Syntax error near unexpected token `&&`\n");
		return (0);
	}
	return (1);
}

int	check_unsupported_operators(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD && current->value)
		{
			if (ft_strcmp(current->value, "&") == 0)
			{
				printf("Syntax error near unexpected token `&`\n");
				return (0);
			}
			if (!syntax_error_and_token(current))
				return (0);
		}
		current = current->next;
	}
	return (1);
}
