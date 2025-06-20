/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:15:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/13 20:43:34 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"

static char	*get_target_dir(t_shell *shell, char **args, char **old_pwd)
{
	if (!args[1])
		return (handle_home_dir(shell->env, old_pwd));
	else if (ft_strcmp(args[1], "-") == 0)
		return (handle_oldpwd_dir(shell->env, old_pwd));
	else
		return (args[1]);
}

int	builtin_cd(t_shell *shell, char **args)
{
	char	*target_dir;
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		ft_putendl_fd("cd: error retrieving current directory", 2);
		return (1);
	}
	target_dir = get_target_dir(shell, args, &old_pwd);
	if (!target_dir)
		return (1);
	if (chdir(target_dir) != 0)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(target_dir, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		free(old_pwd);
		return (1);
	}
	update_pwd_env(shell, old_pwd);
	free(old_pwd);
	return (0);
}
