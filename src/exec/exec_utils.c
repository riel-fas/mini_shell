/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:45:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/13 20:33:19 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

char	*find_command_path(char **paths, char *cmd)
{
	char	*path;
	char	*full_path;
	int		i;

	// If command contains '/', it's a direct path
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
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
			return (full_path);

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
