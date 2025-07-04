/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 20:04:54 by roubelka          #+#    #+#             */
/*   Updated: 2025/07/04 00:00:00 by codespace       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"

static void	process_token(t_token *token, t_cmds **cmd,
			t_cmds **head, t_cmds **tail)
{
	if (token->type == TOKEN_WORD || token->type == TOKEN_ARGUMENT)
	{
		if (!*cmd)
			*cmd = init_new_command(head, tail);
		if (should_word_split(token->original_value, token->value))
			add_split_args_to_cmd(*cmd, token->value);
		else
			add_arg_to_cmd(*cmd, token->value);
	}
	else if (token->type == TOKEN_SINGLE_QUOTED
		|| token->type == TOKEN_DOUBLE_QUOTED)
	{
		if (!*cmd)
			*cmd = init_new_command(head, tail);
		add_arg_to_cmd(*cmd, token->value);
	}
	else if (token->type == TOKEN_PIPE || token->type == TOKEN_SEMICOLON)
		*cmd = NULL;
}

static int	is_redirection(int type)
{
	return (type == TOKEN_REDIR_IN
		|| type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND
		|| type == TOKEN_REDIR_READ_WRITE
		|| type == TOKEN_HEREDOC);
}

t_cmds	*parse_tokens(t_token *tokens)
{
	t_cmds	*head;
	t_cmds	*tail;
	t_cmds	*current_cmd;

	head = NULL;
	tail = NULL;
	current_cmd = NULL;
	while (tokens)
	{
		if (is_redirection(tokens->type))
		{
			if (!current_cmd)
				current_cmd = init_new_command(&head, &tail);
			handle_redirections(current_cmd, &tokens);
			continue ;
		}
		process_token(tokens, &current_cmd, &head, &tail);
		tokens = tokens->next;
	}
	return (head);
}
