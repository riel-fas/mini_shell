/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:45:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/24 21:14:49 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "mini_shell.h"
# include "builtins.h"
# include <sys/stat.h>

/* Executor functions */
int		execute_commands(t_shell *shell, t_cmds *commands);

/* Redirection functions */
int		setup_redirections(t_cmds *cmd);
void	reset_redirections(int stdin_backup, int stdout_backup);

/* Command execution functions */
int		execute_single_command(t_shell *shell, t_cmds *cmd);
int		execute_pipeline(t_shell *shell, t_cmds *commands);

/* Command path functions */
char	*find_command_path(char **paths, char *cmd);

/* Error handling */
void	error_message(char *cmd, char *msg);

/* Memory management */
void	free_env_array(char **env_array);

#endif
