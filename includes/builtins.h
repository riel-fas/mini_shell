/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:15:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/01 12:41:31 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "mini_shell.h"

/* Builtin function types */
typedef int	(*t_builtin_func)(t_shell *shell, char **args);

int				is_builtin(char *cmd);
t_builtin_func	get_builtin(char *cmd);

/* CD utility functions */
void			update_pwd_var(t_env **env, char *pwd_name, char *value);
void			update_pwd_env(t_shell *shell, char *old_pwd);
char			*handle_home_dir(t_env *env, char **old_pwd);
char			*handle_oldpwd_dir(t_env *env, char **old_pwd);
char			*handle_tilde_expansion(t_env *env, char *path, char **old_pwd, int *should_free_target);

int				builtin_cd(t_shell *shell, char **args);
int				builtin_echo(t_shell *shell, char **args);
int				builtin_pwd(t_shell *shell, char **args);
int				builtin_export(t_shell *shell, char **args);
int				builtin_unset(t_shell *shell, char **args);
int				builtin_env(t_shell *shell, char **args);
int				builtin_exit(t_shell *shell, char **args);
int				builtin_colon(t_shell *shell, char **args);

#endif
