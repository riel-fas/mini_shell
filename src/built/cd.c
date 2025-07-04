/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:15:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/03 23:37:53 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"

static int	check_invalid_option(char *arg)
{
	if (arg[0] == '-' && arg[1] == '-' && arg[2] != '\0')
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd(": invalid option", 2);
		return (1);
	}
	return (0);
}

static char	*get_target_dir(t_shell *shell, char **args, char **old_pwd,
		int *should_free_target)
{
	*should_free_target = 0;
	if (!args[1])
		return (handle_home_dir(shell->env, old_pwd));
	else if (ft_strcmp(args[1], "-") == 0)
		return (handle_dash_arg(shell, args, old_pwd));
	else if (ft_strcmp(args[1], "--") == 0)
		return (handle_double_dash_arg(shell, args, old_pwd,
				should_free_target));
	else
		return (handle_regular_arg(shell, args, old_pwd, should_free_target));
}

static int	handle_chdir_error(char *target_dir, char *old_pwd,
		int should_free_target)
{
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd(target_dir, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
	if (should_free_target)
		free(target_dir);
	free(old_pwd);
	return (1);
}

int	builtin_cd(t_shell *shell, char **args)
{
	char	*target_dir;
	char	*old_pwd;
	int		should_free_target;

	if (args[1] && check_invalid_option(args[1]))
		return (1);
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		ft_putendl_fd("cd: error retrieving current directory", 2);
		return (1);
	}
	target_dir = get_target_dir(shell, args, &old_pwd, &should_free_target);
	if (!target_dir)
		return (1);
	if (chdir(target_dir) != 0)
		return (handle_chdir_error(target_dir, old_pwd, should_free_target));
	if (should_free_target)
		free(target_dir);
	update_pwd_env(shell, old_pwd);
	free(old_pwd);
	return (0);
}
