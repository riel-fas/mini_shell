/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_extraction_operator.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 19:15:00 by codespace        #+#    #+#             */
/*   Updated: 2025/07/04 00:00:00 by codespace       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"

/* Operator token creation */
static char	*create_operator_token(char *op)
{
	return (ft_strdup(op));
}

/* Double-character operator handling */
static char	*handle_double_operator(char *input, int *i,
			t_token_type *type, char *op)
{
	if (ft_strncmp(&input[*i], op, 2) == 0)
	{
		if (op[0] == '>')
			set_operator_context(i, 2, type, TOKEN_REDIR_APPEND);
		else if (op[0] == '<' && op[1] == '<')
			set_operator_context(i, 2, type, TOKEN_HEREDOC);
		else
			set_operator_context(i, 2, type, TOKEN_REDIR_READ_WRITE);
		return (create_operator_token(op));
	}
	return (NULL);
}

/* Single-character operator handling */
static char	*handle_single_operator(char *input, int *i,
			t_token_type *type, char op)
{
	if (input[*i] == op)
	{
		if (op == '>')
			set_operator_context(i, 1, type, TOKEN_REDIR_OUT);
		else if (op == '<')
			set_operator_context(i, 1, type, TOKEN_REDIR_IN);
		else if (op == '|')
			set_operator_context(i, 1, type, TOKEN_PIPE);
		else
			set_operator_context(i, 1, type, TOKEN_SEMICOLON);
		return (create_operator_token(&op));
	}
	return (NULL);
}

/* Main operator extraction */
char	*extract_operator(char *input, int *i, t_token_type *type)
{
	char	*op;

	op = handle_double_operator(input, i, type, ">>");
	if (op)
		return (op);
	op = handle_double_operator(input, i, type, "<<");
	if (op)
		return (op);
	op = handle_double_operator(input, i, type, "<>");
	if (op)
		return (op);
	op = handle_single_operator(input, i, type, '>');
	if (op)
		return (op);
	op = handle_single_operator(input, i, type, '<');
	if (op)
		return (op);
	op = handle_single_operator(input, i, type, '|');
	if (op)
		return (op);
	op = handle_single_operator(input, i, type, ';');
	if (op)
		return (op);
	*type = TOKEN_WORD;
	return (NULL);
}
