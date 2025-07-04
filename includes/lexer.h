/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 20:25:41 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/03 20:58:03 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "mini_shell.h"

int		is_whitespace(char c);
int		is_operator(char c);
int		is_quote(char c);
int		all_tokens_empty(t_token *tokens);
t_token	*create_token(char *value, t_token_type type);
void	add_token(t_token **tokens, t_token *new_token);
void	free_tokens(t_token *tokens);
char	*extract_word(char *input, int *i);
char	*extract_operator(char *input, int *i, t_token_type *type);
char	*extract_quoted_string(char *input, int *i, char quote);
t_token	*tokenize(char *input);
void	print_tokens(t_token *tokens);
char	*expand_variables(char *str, t_env *env, int exit_status,
			int in_quotes);
void	expand_tokens(t_token *tokens, t_env *env, int exit_status);

#endif
