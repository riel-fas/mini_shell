/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 12:45:00 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 17:59:14 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_H
# define MEMORY_H

# include <stdlib.h>
# include <unistd.h>

/* Forward declarations for structs defined in mini_shell.h */
struct s_token;
struct s_heredoc_list;
struct s_cmds;
struct s_env;
struct s_shell;

typedef struct s_token t_token;
typedef struct s_heredoc_list t_heredoc_list;
typedef struct s_cmds t_cmds;
typedef struct s_env t_env;
typedef struct s_shell t_shell;

/* Memory management functions */
void	free_str_array(char **arr);
void	free_token_node(t_token *token);
void	free_token_list(t_token *tokens);
void	free_heredoc_list(t_heredoc_list *heredoc_list);
void	free_command_node(t_cmds *cmd);
void	free_command_list(t_cmds *commands);
void	free_env_node(t_env *env_node);
void	free_env_list(t_env *env);
void	complete_shell_cleanup(t_shell *shell);
void	safe_free(void **ptr);

#endif
