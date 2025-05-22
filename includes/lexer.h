/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 20:25:41 by riel-fas          #+#    #+#             */
/*   Updated: 2025/05/22 17:49:31 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "mini_shell.h"

//leer_utils.c
int		is_whitespace(char c);
int		is_operator(char c);
int		is_quote(char c);
t_token	*create_token(char *value, t_token_type type);
void	add_token(t_token **tokens, t_token *new_token);
void	free_tokens(t_token *tokens);

//lexer_extraction.c
char	*extract_word(char *input, int *i);
char	*extract_operator(char *input, int *i, t_token_type *type);
char	*extract_quoted_string(char *input, int *i, char quote);

//lexer.c
t_token	*tokenize(char *input);
void	print_tokens(t_token *tokens);

#endif
