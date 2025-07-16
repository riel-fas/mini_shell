/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokeniser_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:14:21 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:14:28 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

t_token	*ft_get_next_token_ctx(t_next_token_ctx *ctx);

t_lexer	*lexer_init(char *input)
{
	t_lexer	*lexer;

	lexer = malloc(sizeof(t_lexer));
	lexer->input = input;
	lexer->pos = 0;
	lexer->len = ft_strlen(input);
	return (lexer);
}

t_token	*is_string_token(char *input, t_list *minienv, t_shell *shell)
{
	t_lexer	*lexer;
	t_token	*token;
	t_token	*token_list;
	bool	heredoc;

	if (input == NULL)
		return (NULL);
	lexer = lexer_init(input);
	token_list = NULL;
	heredoc = false;
	token = ft_get_next_token_ctx(&(t_next_token_ctx){lexer,
			minienv, &heredoc, shell});
	while (token)
	{
		if (!token)
			break ;
		if (token->value)
			add_toekn_back(&token_list, new_token_set(token));
		free_token(token);
		token = ft_get_next_token_ctx(&(t_next_token_ctx){lexer,
				minienv, &heredoc, shell});
	}
	free(lexer);
	return (token_list);
}

bool	to_join(t_lexer *lexer)
{
	return (lexer->input[lexer->pos] != ' ' && lexer->input[lexer->pos] != '\0'
		&& !is_red_pip(lexer->input[lexer->pos]));
}
