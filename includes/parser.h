/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 05:36:05 by roubelka          #+#    #+#             */
/*   Updated: 2025/07/04 21:32:27 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "mini_shell.h"

//parser_cmd.c
t_cmds		*init_new_command(t_cmds **head, t_cmds **tail);
void		add_arg_to_cmd(t_cmds *cmd, char *value);
void		add_split_args_to_cmd(t_cmds *cmd, char *value);

//parser_main.c
t_cmds		*parse_tokens(t_token *tokens);

//parser_redirections.c
int			contains_unquoted_variables(char *str);
int			should_word_split(char *orig_val, char *exp_val);
void		handle_redirections(t_cmds *cmd, t_token **tokens);

//parser_utils.c
void		free_heredoc(t_heredoc_list *heredoc_list);
void		free_commands(t_cmds *cmds);

//syntax_error_2.c
int			check_redirection_syntax(t_token *tokens);

//syntax_error.c
int			check_pipe_syntax(t_token *tokens);
int			check_standalone_operators(t_token *tokens);
int			check_unsupported_operators(t_token *tokens);

#endif
