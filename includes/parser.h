/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 05:36:05 by roubelka          #+#    #+#             */
/*   Updated: 2025/06/14 00:51:13 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "mini_shell.h"

int     check_redirection_syntax(t_token *tokens);
int     check_pipe_syntax(t_token *tokens);
// int     check_quotes_closed(t_token *tokens);

t_cmds  *init_new_command(t_cmds **head, t_cmds **tail);
void	add_arg_to_cmd(t_cmds *cmd, char *value);
void    handle_redirections(t_cmds *cmd, t_token **tokens);
t_cmds	*parse_tokens(t_token *tokens);
void    print_commands(t_cmds *cmds);
void    free_commands(t_cmds *cmds);

#endif
