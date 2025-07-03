/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:45:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/01 13:01:03 by riad             ###   ########.fr       */
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
		// First check if file exists
		if (access(cmd, F_OK) == 0)
		{
			// Check if it's a directory
			if (stat(cmd, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
				return (NULL); // Let caller handle "Is a directory" error
			// File exists, now check execute permission
			if (access(cmd, X_OK) == 0)
				return (ft_strdup(cmd));
			// File exists but no execute permission - return special value
			return (ft_strdup("PERMISSION_DENIED"));
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
