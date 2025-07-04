/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 00:36:10 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 03:45:04 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

static char	*check_absolute_path(char *cmd)
{
	struct stat	file_stat;

	if (access(cmd, F_OK) != 0)
		return (NULL);
	if (stat(cmd, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
		return (NULL);
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

static char	*create_full_path(char *dir, char *cmd)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	return (full_path);
}

static char	*check_path_executable(char *full_path)
{
	struct stat	file_stat;

	if (access(full_path, F_OK) != 0)
		return (NULL);
	if (stat(full_path, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
		return (NULL);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	return (NULL);
}

static char	*search_in_paths(char **paths, char *cmd)
{
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		full_path = create_full_path(paths[i], cmd);
		if (!full_path)
		{
			free_env_array(paths);
			return (NULL);
		}
		if (check_path_executable(full_path))
		{
			free_env_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*get_command_path(char *cmd, t_env *env)
{
	char	*path_env;
	char	**paths;
	char	*result;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (check_absolute_path(cmd));
	path_env = get_env_value(env, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	result = search_in_paths(paths, cmd);
	if (!result)
		free_env_array(paths);
	return (result);
}
