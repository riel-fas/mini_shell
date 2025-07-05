/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell_loop_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 02:18:34 by codespace         #+#    #+#             */
/*   Updated: 2025/07/05 02:24:16 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"
#include "../includes/lexer.h"

static void	print_redir_syntax_error(const char *value)
{
	char	c;
	int		i;

	if (value && value[0])
	{
		c = value[0];
		i = 0;
		while (value[i] && value[i] == c)
			i++;
		if (value[i] == '\0' && (c == '>' || c == '<'))
			printf("Syntax error near unexpected token `%s`\n", value);
		else if ((c == '>' || c == '<') && value[1] == '\0')
			printf("Syntax error near unexpected token `%c`\n", c);
		else
			printf("Syntax error near unexpected token `%c`\n", c);
	}
	else
		printf("Syntax error near unexpected token `newline`\n");
}

int	check_first_token_redirection(t_shell *shell)
{
	t_token	*first;

	first = shell->tokens;
	if (first && (first->type == TOKEN_REDIR_OUT
			|| first->type == TOKEN_REDIR_IN
			|| first->type == TOKEN_REDIR_APPEND
			|| first->type == TOKEN_HEREDOC
			|| first->type == TOKEN_REDIR_READ_WRITE))
	{
		print_redir_syntax_error(first->value);
		shell->exit_status = 2;
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		return (0);
	}
	return (1);
}

int	check_general_syntax(t_shell *shell)
{
	if (!check_redirection_syntax(shell->tokens)
		|| !check_pipe_syntax(shell->tokens)
		|| !check_unsupported_operators(shell->tokens))
	{
		shell->exit_status = 2;
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		return (0);
	}
	return (1);
}
