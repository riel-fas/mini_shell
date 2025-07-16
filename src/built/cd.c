/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:31:41 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:31:49 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

void	path_recheck(t_list *env, char *new_path, char *old_path)
{
	t_list	*old_pwd;
	t_list	*current_pwd;
	char	*tmp;
	char	*cwd;

	free_every_char(new_path);
	tmp = NULL;
	cwd = getcwd(NULL, 0);
	old_pwd = node(env, "OLDPWD");
	current_pwd = node(env, "PWD");
	if (old_pwd)
	{
		if (old_pwd->value)
			free(old_pwd->value);
		old_pwd->value = ft_strdup(old_path);
	}
	free(old_path);
	if (current_pwd)
	{
		tmp = current_pwd->value;
		current_pwd->value = cwd;
		free(tmp);
	}
	else
		free(cwd);
}

static char	*handel_tilde(char *cmd, t_list *env)
{
	char	*home;
	char	*tmp;

	tmp = NULL;
	if (cmd[0] == '~')
	{
		home = env_getting("HOME", env);
		if (home == NULL)
			return (NULL);
		tmp = my_strjoin(home, cmd + 1);
	}
	else
		tmp = cmd;
	return (tmp);
}

static int	get_target_path(char **cmd, char **path,
			char *old_path, t_list **env)
{
	char	*tmp;

	tmp = NULL;
	if (cmd[1] == NULL || ft_strcmp(cmd[1], "~") == 0)
	{
		*path = env_getting("HOME", *env);
		if (*path == NULL)
			return (free(old_path), write(2, CD_HOME_ERR, 28), 1);
	}
	else if (ft_strcmp(cmd[1], "-") == 0)
	{
		*path = env_getting("OLDPWD", *env);
		if (*path == NULL)
			return (free(old_path), write(2, CD_OLDPWD_ERR, 30), 1);
	}
	else
	{
		tmp = handel_tilde(cmd[1], *env);
		if (tmp == NULL)
			return (free(old_path), write(2, CD_HOME_ERR, 28), 1);
		*path = ft_strdup(tmp);
		if (tmp != cmd[1])
			free(tmp);
	}
	return (0);
}

int	cd_built(char **cmd, t_list **env)
{
	char	*path;
	char	*old_path;
	int		is_dash;

	path = NULL;
	old_path = env_getting("PWD", *env);
	is_dash = (cmd[1] != NULL && ft_strcmp(cmd[1], "-") == 0);
	if (size_cd(cmd) > 2)
		return (free(old_path), write(2, "minishell :cd: too many arguments\n",
				34), 1);
	if (get_target_path(cmd, &path, old_path, env) != 0)
		return (free(path), 1);
	if (path == NULL)
		return (free(old_path), write(2, "minishell :cd: HOME not set\n", 28),
			1);
	if (chdir(path) != 0)
		return (perror(path), free(path), free(old_path), 1);
	if (is_dash)
	{
		write(1, path, ft_strlen(path));
		write(1, "\n", 1);
	}
	path_recheck(*env, path, old_path);
	return (0);
}
