/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 20:45:00 by riad              #+#    #+#             */
/*   Updated: 2025/07/03 23:37:53 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"

char	*handle_dash_arg(t_shell *shell, char **args, char **old_pwd)
{
	if (args[2])
	{
		ft_putendl_fd("cd: too many arguments", 2);
		free(*old_pwd);
		return (NULL);
	}
	return (handle_oldpwd_dir(shell->env, old_pwd));
}

char	*handle_double_dash_arg(t_shell *shell, char **args,
		char **old_pwd, int *should_free_target)
{
	if (args[2])
	{
		if (args[3])
		{
			ft_putendl_fd("cd: too many arguments", 2);
			free(*old_pwd);
			return (NULL);
		}
		return (handle_tilde_expansion(shell->env, args[2], old_pwd,
				should_free_target));
	}
	else
		return (handle_home_dir(shell->env, old_pwd));
}

char	*handle_regular_arg(t_shell *shell, char **args,
		char **old_pwd, int *should_free_target)
{
	if (args[2])
	{
		ft_putendl_fd("cd: too many arguments", 2);
		free(*old_pwd);
		return (NULL);
	}
	return (handle_tilde_expansion(shell->env, args[1], old_pwd,
			should_free_target));
}

char	*expand_tilde_with_home(t_env *env, char *path, char **old_pwd,
		int *should_free_target)
{
	char	*home_dir;
	char	*expanded_path;

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
