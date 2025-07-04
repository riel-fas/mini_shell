/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 03:30:41 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/04 03:46:10 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

static char	*handle_absolute_path(char *cmd)
{
	struct stat	file_stat;

	if (access(cmd, F_OK) == 0)
	{
		if (stat(cmd, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
			return (NULL);
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (ft_strdup("PERMISSION_DENIED"));
	}
	return (NULL);
}

static int	is_valid_executable(char *full_path)
{
	struct stat	file_stat;

	if (access(full_path, X_OK) == 0)
	{
		if (stat(full_path, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
			return (0);
		return (1);
	}
	return (0);
}

static char	*search_in_path_dirs(char **paths, char *cmd)
{
	char	*path;
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(path, cmd);
		free(path);
		if (!full_path)
			return (NULL);
		if (is_valid_executable(full_path))
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*find_command_path(char **paths, char *cmd)
{
	if (ft_strchr(cmd, '/'))
		return (handle_absolute_path(cmd));
	if (!paths)
		return (NULL);
	return (search_in_path_dirs(paths, cmd));
}

void	error_message(char *cmd, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
}
