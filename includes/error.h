/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 12:48:00 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 19:16:43 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>

/* Forward declarations for structs defined in mini_shell.h */
struct s_env;
struct s_cmds;
struct s_shell;

typedef struct s_env t_env;
typedef struct s_cmds t_cmds;
typedef struct s_shell t_shell;

/* Enhanced error handling functions */
void	enhanced_exec_error(char *cmd_path, char *cmd_name);
void	enhanced_redir_error(char *file, char *msg, int err);
void	ambiguous_redirect_error(char *file);
void	enhanced_syntax_error(char *msg, char *token, int line_num);
void    ft_cmd_error(char *cmd, char *error, int status);

/* Error detection functions */
int		check_command_errors(char *cmd, t_env *env);
int		check_redirection_errors(t_cmds *cmd);
int		check_pipeline_errors(t_cmds *commands, t_env *env);
int		detect_execution_errors(t_shell *shell, t_cmds *commands);

#endif
