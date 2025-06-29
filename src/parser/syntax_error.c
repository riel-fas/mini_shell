/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 05:35:11 by roubelka          #+#    #+#             */
/*   Updated: 2025/06/29 17:52:13 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"

int check_redirection_syntax(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == TOKEN_REDIR_OUT || tokens->type == TOKEN_REDIR_APPEND ||
			tokens->type == TOKEN_REDIR_IN || tokens->type == TOKEN_HEREDOC ||
			tokens->type == TOKEN_REDIR_READ_WRITE)
		{
			if (!tokens->next)
			{
				printf("Syntax error near unexpected token `newline`\n");
				return 0;
			}
			if (tokens->next->type == TOKEN_REDIR_OUT ||
				tokens->next->type == TOKEN_REDIR_APPEND ||
				tokens->next->type == TOKEN_REDIR_IN ||
				tokens->next->type == TOKEN_HEREDOC ||
				tokens->next->type == TOKEN_REDIR_READ_WRITE ||
				tokens->next->type == TOKEN_PIPE)
			{
				if (tokens->next->type == TOKEN_PIPE)
					printf("Syntax error near unexpected token `|`\n");
				else
					printf("Syntax error near unexpected token `%s`\n", tokens->next->value);
				return 0;
			}
		 }
		tokens = tokens->next;
	}
	return 1;
}

int check_pipe_syntax(t_token *tokens)
{
	if (!tokens)
		return 1;
	if (tokens->type == TOKEN_PIPE)
	{
		if (tokens->next && tokens->next->type == TOKEN_PIPE)
			printf("Syntax error near unexpected token `||`\n");
		else
			printf("Syntax error near unexpected token `|`\n");
	return 0;
	}
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE)
		{
			if (!tokens->next)
			{
				printf("Syntax error near unexpected token `newline`\n");
				return 0;
			}
			else if (tokens->next->type == TOKEN_PIPE)
			{
				printf("Syntax error near unexpected token `||`\n");
				return 0;
			}
		}
		tokens = tokens->next;
	}
	return 1;
}

int check_standalone_operators(t_token *tokens)
{
	t_token *current = tokens;

	while (current)
	{
		if (current->type == TOKEN_WORD && ft_strcmp(current->value, "!") == 0)
		{
			if (!current->next || current->next->type == TOKEN_PIPE)
				return (0); // Syntax error - exit silently like bash
		}
		current = current->next;
	}
	return (1);
}


int check_unsupported_operators(t_token *tokens)
{
	t_token *current = tokens;

	while (current)
	{
		if (current->type == TOKEN_WORD && current->value)
		{
			if (ft_strcmp(current->value, "&") == 0)
			{
				printf("Syntax error near unexpected token `&`\n");
				return (0);
			}
			else if (ft_strcmp(current->value, "&&") == 0 || (ft_strlen(current->value) > 1 && current->value[0] == '&' && current->value[1] == '&'))
			{
				printf("Syntax error near unexpected token `&&`\n");
				return (0);
			}
			else if (current->value[0] == '&' && ft_strlen(current->value) > 2)
			{
				printf("Syntax error near unexpected token `&&`\n");
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}
