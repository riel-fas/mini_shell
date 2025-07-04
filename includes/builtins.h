/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:15:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/04 21:28:08 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "mini_shell.h"

/* Builtin function types */
typedef int	(*t_builtin_func)(t_shell *shell, char **args);

//cd_utils.c
void	update_pwd_var(t_env **env, char *pwd_name, char *value);
void	update_pwd_env(t_shell *shell, char *old_pwd);
char	*handle_home_dir(t_env *env, char **old_pwd);
char	*handle_oldpwd_dir(t_env *env, char **old_pwd);
char	*handle_tilde_expansion(t_env *env, char *path, char **old_pwd,
			int *should_free_target);

//cd_utils_2.c
char	*handle_dash_arg(t_shell *shell, char **args, char **old_pwd);
char	*handle_double_dash_arg(t_shell *shell, char **args,
			char **old_pwd, int *should_free_target);
char	*handle_regular_arg(t_shell *shell, char **args,
			char **old_pwd, int *should_free_target);
char	*expand_tilde_with_home(t_env *env, char *path, char **old_pwd,
			int *should_free_target);

//cd.c
int		builtin_cd(t_shell *shell, char **args);

//colon.c
int		builtin_colon(t_shell *shell, char **args);

//echo.c
int		builtin_echo(t_shell *shell, char **args);

//env.c
int		builtin_env(t_shell *shell, char **args);

//exit.c
int		builtin_exit(t_shell *shell, char **args);

//export_utils.c
int		parse_assignment(char *arg, char **name, char **value);
int		process_export_arg(t_shell *shell, char *arg, int *path_modified);

//export.c
int		builtin_export(t_shell *shell, char **args);

//pwd.c
int		builtin_pwd(t_shell *shell, char **args);

//unset_utils.c
void	remove_first_node(t_env **env_list, t_env *current);
void	remove_middle_node(t_env *prev, t_env *current);
void	remove_env_var(t_env **env_list, char *key);
int		is_valid_varname(char *name);
void	handle_invalid_varname(char *arg, int *status);

//unset.c
int		builtin_unset(t_shell *shell, char **args);

#endif
