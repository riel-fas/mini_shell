/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 04:06:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/04 05:19:56 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"

static void	handle_var_expansion(int *i, t_expansion_ctx *ctx)
{
	char	*var_name;
	char	*var_value;

	(*i)++;
	var_name = extract_var_name(ctx->str, i);
	if (var_name)
	{
		var_value = get_var_value(var_name, ctx->env, ctx->exit_status);
		if (var_value)
		{
			*(ctx->j) = copy_var_value(
					var_value, ctx->result, *(ctx->j), ctx->result_size);
			free(var_value);
		}
		free(var_name);
	}
}

static void	expand_variables_loop(t_expansion_ctx *ctx)
{
	int	in_single;
	int	in_double;
	int	i;

	in_single = 0;
	in_double = 0;
	i = 0;
	while (ctx->str[i])
	{
		if ((ctx->str[i] == '\'' && !in_double)
			|| (ctx->str[i] == '"' && !in_single))
			toggle_quotes(ctx->str[i], &in_single, &in_double);
		else if (ctx->str[i] == '$' && !in_single && ctx->str[i + 1]
			&& is_var_start(ctx->str[i + 1]))
			handle_var_expansion(&i, ctx);
		else if (ctx->str[i] == '$' && !in_single && ctx->str[i + 1]
			&& (ctx->str[i + 1] == '"' || ctx->str[i + 1] == '\''))
			i++;
		else
			ctx->result[(*ctx->j)++] = ctx->str[i++];
	}
}

static void	init_expansion_ctx(t_expansion_ctx *ctx, t_init_ctx_args *args)
{
	ctx->str = args->str;
	ctx->result = args->result;
	ctx->j = args->j;
	ctx->result_size = args->result_size;
	ctx->env = args->env;
	ctx->exit_status = args->exit_status;
}

char	*expand_variables(char *str, t_env *env, int exit_status, int in_quotes)
{
	t_expand_vars_locals	v;
	t_expansion_ctx			ctx;
	t_init_ctx_args			args;

	(void)in_quotes;
	if (!str)
		return (NULL);
	v.len = ft_strlen(str);
	v.result_size = v.len * 10 + 1024;
	v.result = malloc(v.result_size);
	if (!v.result)
		return (NULL);
	v.j = 0;
	args.str = str;
	args.result = v.result;
	args.j = &v.j;
	args.result_size = v.result_size;
	args.env = env;
	args.exit_status = exit_status;
	init_expansion_ctx(&ctx, &args);
	expand_variables_loop(&ctx);
	v.result[v.j] = '\0';
	v.final_result = ft_strdup(v.result);
	free(v.result);
	return (v.final_result);
}

void	expand_tokens(t_token *tokens, t_env *env, int exit_status)
{
	t_token	*current;
	char	*expanded;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			expanded = expand_variables(current->value, env, exit_status, 0);
			if (expanded)
			{
				free(current->value);
				current->value = expanded;
			}
		}
		current = current->next;
	}
}
