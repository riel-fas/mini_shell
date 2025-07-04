/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:29:57 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 18:30:33 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"

static int	syntax_error_redir_next(t_token *tokens)
{
	if (!tokens->next)
	{
		printf("Syntax error near unexpected token `newline`\n");
		return (0);
	}
	if (tokens->next->type == TOKEN_REDIR_OUT
		|| tokens->next->type == TOKEN_REDIR_APPEND
		|| tokens->next->type == TOKEN_REDIR_IN
		|| tokens->next->type == TOKEN_HEREDOC
		|| tokens->next->type == TOKEN_REDIR_READ_WRITE
		|| tokens->next->type == TOKEN_PIPE)
	{
		if (tokens->next->type == TOKEN_PIPE)
			printf("Syntax error near unexpected token `|`\n");
		else
			printf("Syntax error near unexpected token `%s`\n",
				tokens->next->value);
		return (0);
	}
	return (1);
}

int	check_redirection_syntax(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == TOKEN_REDIR_OUT
			|| tokens->type == TOKEN_REDIR_APPEND
			|| tokens->type == TOKEN_REDIR_IN
			|| tokens->type == TOKEN_HEREDOC
			|| tokens->type == TOKEN_REDIR_READ_WRITE)
		{
			if (!syntax_error_redir_next(tokens))
				return (0);
		}
		tokens = tokens->next;
	}
	return (1);
}
