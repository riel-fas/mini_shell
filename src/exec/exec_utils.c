/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:45:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/24 21:14:49 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

char	*find_command_path(char **paths, char *cmd)
{
	char	*path;
	char	*full_path;
	int		i;
	struct stat	file_stat;

	// If command contains '/', it's a direct path
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
		{
			// Check if it's a directory
			if (stat(cmd, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
				return (NULL); // Let caller handle "Is a directory" error
			return (ft_strdup(cmd));
		}
		return (NULL);
	}

	// Check in PATH
	if (!paths)
		return (NULL);

	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(path, cmd);
		free(path);

		if (!full_path)
			return (NULL);

		if (access(full_path, X_OK) == 0)
		{
			// Check if it's a directory
			if (stat(full_path, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
			{
				free(full_path);
				continue; // Skip directories, keep looking
			}
			return (full_path);
		}

		free(full_path);
		i++;
	}

	return (NULL);
}

void	error_message(char *cmd, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
}

void	free_env_array(char **env_array)
{
	int	i;

	if (!env_array)
		return ;
	i = 0;
	while (env_array[i])
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}
