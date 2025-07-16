/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:13:54 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:14:04 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

void	token_init_arg(t_token *token)
{
	token->variable = false;
	token->current = NULL;
	token->next = NULL;
	token->prev = NULL;
}

t_token	*handle_tok_quotes(t_lexer *lexer, char quote_char, t_shell *shell)
{
	int		start;
	char	*content;
	t_token	*token;

	start = ++lexer->pos;
	while (lexer->pos < lexer->len && lexer->input[lexer->pos] != quote_char)
		lexer->pos++;
	if (lexer->pos >= lexer->len)
		return (printf(UNCLOSED), shell->exit_status = 258, NULL);
	content = ft_substr(lexer->input, start, lexer->pos - start);
	lexer->pos++;
	token = (t_token *)malloc(sizeof(t_token));
	if (!token || !content)
		return (NULL);
	if (quote_char == '\'')
		token->type = SINGLE_QUOTE;
	else
		token->type = DOUBLE_QUOTE;
	token->value = content;
	if (ft_isspace(lexer->input[lexer->pos]))
		token->addspace = true;
	else
		token->addspace = false;
	token_init_arg(token);
	return (token);
}

void	token_type_set(t_token *token, int op_len, char *op, bool *heredoc)
{
	if (op_len == 2)
	{
		if (op[0] == '>')
			token->type = APPEND;
		else
		{
			token->type = HEREDOC;
			*heredoc = true;
		}
	}
	else if (op[0] == '|')
		token->type = PIPE;
	else
	{
		if (op[0] == '>')
			token->type = OUTPUT;
		else
			token->type = INPUT;
	}
}

t_token	*ft_handle_operator(t_lexer *lexer, bool *heredoc)
{
	char	op[3];
	int		op_len;
	t_token	*token;

	op[0] = lexer->input[lexer->pos];
	op[1] = '\0';
	op_len = 1;
	if (check_duplicated(lexer, op))
	{
		op[1] = lexer->input[lexer->pos + 1];
		op[2] = '\0';
		op_len = 2;
	}
	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(op);
	if (!token->value)
		return (free(token), NULL);
	token->addspace = false;
	token_init_arg(token);
	token_type_set(token, op_len, op, heredoc);
	lexer->pos += op_len;
	return (token);
}

t_token	*word_handling(t_lexer *lexer)
{
	int		start;
	char	*value;
	t_token	*token;

	start = lexer->pos;
	while (lexer->pos < lexer->len && !ft_isspace(lexer->input[lexer->pos])
		&& !is_red_pip(lexer->input[lexer->pos])
		&& lexer->input[lexer->pos] != '\'' && lexer->input[lexer->pos] != '"')
		lexer->pos++;
	value = ft_substr(lexer->input, start, lexer->pos - start);
	token = (t_token *)malloc(sizeof(t_token));
	if (!token || !value)
		return (NULL);
	token->type = WORD;
	token->value = value;
	if (ft_isspace(lexer->input[lexer->pos]))
		token->addspace = true;
	else
		token->addspace = false;
	token_init_arg(token);
	return (token);
}
