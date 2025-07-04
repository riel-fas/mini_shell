/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 20:25:41 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/04 05:25:01 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "mini_shell.h"


typedef struct s_expansion_ctx {
	char	*str;
	char	*result;
	int		*j;
	int		result_size;
	t_env	*env;
	int		exit_status;
} t_expansion_ctx;

typedef struct s_expand_vars_locals {
	char	*result;
	int		len;
	int		j;
	int		result_size;
	char	*final_result;
} t_expand_vars_locals;

typedef struct s_init_ctx_args {
	char *str;
	char *result;
	int *j;
	int result_size;
	t_env *env;
	int exit_status;
} t_init_ctx_args;

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
char	*extract_var_name(char *input, int *i);
char	*get_var_value(char *var_name, t_env *env, int exit_status);

int     copy_var_value(char *var_value, char *result, int j, int result_size);
void    toggle_quotes(int c, int *in_single, int *in_double);
int     is_var_start(char c);
int	add_char_to_buffer(char **result, size_t *capacity, char c);
void	set_operator_context(int *i, int inc, t_token_type *type,
			t_token_type token_type);



#endif
