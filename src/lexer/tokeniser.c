/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokeniser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:13:05 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:13:13 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

t_token	*ft_get_next_token_ctx(t_next_token_ctx *ctx);

t_token	*ft_jointok_ctx_finish(t_token *token, t_token *n)
{
	char	*value;

	if (ft_strlen(token->value) > 0
		&& token->value[ft_strlen(token->value) - 1] == '$'
		&& (n->type == DOUBLE_QUOTE || n->type == SINGLE_QUOTE))
	{
		return (dollar_sign_joining(token, n));
	}
	if (token->type != WORD)
		n->type = token->type;
	value = ft_strjoin(token->value, n->value);
	free_token(token);
	free(n->value);
	n->value = value;
	return (n);
}

t_token	*ft_jointok_ctx(t_token *token, t_x *ctx)
{
	t_token	*n;

	if (*(ctx->heredoc) == false)
		variables_expanding(&token, ctx->minienv, ctx->shell);
	n = ft_get_next_token_ctx(
			&(t_next_token_ctx){
			*(ctx->lexer),
			ctx->minienv,
			ctx->heredoc,
			ctx->shell
		}
			);
	if (!n && ctx->shell->exit_status == 258)
		return (free_token(token), NULL);
	if (*(ctx->heredoc) == false)
		variables_expanding(&n, ctx->minienv, ctx->shell);
	return (ft_jointok_ctx_finish(token, n));
}

static t_token	*ft_handle_token_type(t_next_token_ctx *ctx, char current_char)
{
	t_token	*token;

	if (current_char == '\'' || current_char == '"')
	{
		token = handle_tok_quotes(ctx->lexer, current_char, ctx->shell);
		if (!token)
			return (NULL);
	}
	else
		token = word_handling(ctx->lexer);
	if (to_join(ctx->lexer))
		token = ft_jointok_ctx(
				token,
				&(t_x){
				&(ctx->lexer),
				ctx->minienv,
				ctx->heredoc,
				ctx->shell
			}
				);
	return (token);
}

t_token	*ft_get_next_token_ctx(t_next_token_ctx *ctx)
{
	char	current_char;

	while (ctx->lexer->pos < ctx->lexer->len
		&& ft_isspace(ctx->lexer->input[ctx->lexer->pos]))
		ctx->lexer->pos++;
	if (ctx->lexer->pos >= ctx->lexer->len)
		return (NULL);
	current_char = ctx->lexer->input[ctx->lexer->pos];
	if (is_red_pip(current_char))
		return (ft_handle_operator(ctx->lexer, ctx->heredoc));
	return (ft_handle_token_type(ctx, current_char));
}

int	toksizer(t_token *lst)
{
	t_token	*p;
	int		len;

	if (lst == NULL)
		return (0);
	p = lst;
	len = 0;
	while (p != NULL)
	{
		p = p->next;
		len++;
	}
	return (len);
}
