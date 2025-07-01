/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 00:50:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/01 12:41:31 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"

void	update_pwd_var(t_env **env, char *pwd_name, char *value)
{
	t_env	*var;

	var = find_env_var(*env, pwd_name);
	if (var)
	{
		free(var->value);
		var->value = ft_strdup(value);
	}
	else
		add_env_node(env, new_env_node(pwd_name, value));
}

void	update_pwd_env(t_shell *shell, char *old_pwd)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return ;
	update_pwd_var(&shell->env, "PWD", new_pwd);
	update_pwd_var(&shell->env, "OLDPWD", old_pwd);
	free(new_pwd);
}

char	*handle_home_dir(t_env *env, char **old_pwd)
{
	char	*target_dir;

	target_dir = get_env_value(env, "HOME");
	if (!target_dir)
	{
		ft_putendl_fd("cd: HOME not set", 2);
		free(*old_pwd);
		return (NULL);
	}
	return (target_dir);
}

char	*handle_oldpwd_dir(t_env *env, char **old_pwd)
{
	char	*target_dir;

	target_dir = get_env_value(env, "OLDPWD");
	if (!target_dir)
	{
		ft_putendl_fd("cd: OLDPWD not set", 2);
		free(*old_pwd);
		return (NULL);
	}
	ft_putendl_fd(target_dir, 1);
	return (target_dir);
}

char	*handle_tilde_expansion(t_env *env, char *path, char **old_pwd, int *should_free_target)
{
	char	*home_dir;
	char	*expanded_path;

	*should_free_target = 0;
	if (!path || path[0] != '~')
		return (path);
	if (path[0] == '~' && (path[1] == '\0' || path[1] == '/'))
	{
		home_dir = get_env_value(env, "HOME");
		if (!home_dir)
		{
			ft_putendl_fd("cd: HOME not set", 2);
			free(*old_pwd);
			return (NULL);
		}
		if (path[1] == '\0')
			return (home_dir);
		expanded_path = ft_strjoin(home_dir, &path[1]);
		if (!expanded_path)
		{
			free(*old_pwd);
			return (NULL);
		}
		*should_free_target = 1;
		return (expanded_path);
	}
	return (path);
}
