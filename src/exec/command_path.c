/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 12:10:10 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 17:59:17 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"
#include "../../includes/executor.h"

/**
 * Checks if a path corresponds to a directory
 * @param path Path to check
 * @return true if path is a directory, false otherwise
 */
bool	is_directory(char *path)
{
	struct stat	sb;

	if (!path || path[0] == '\0')
		return (false);
	if (stat(path, &sb) == -1)
		return (false);
	return (S_ISDIR(sb.st_mode));
}

/**
 * Checks if a command path is a full path (absolute or relative) or a command name
 * @param cmd Command to check
 * @return true if cmd is a full path, false if it's just a command name
 */
bool	is_full_path(char *cmd)
{
	if (!cmd || cmd[0] == '\0')
		return (false);
	return (cmd[0] == '/' || (cmd[0] == '.' && (cmd[1] == '/' ||
		(cmd[1] == '.' && cmd[2] == '/'))));
}

/**
 * Handles errors for directory and permission issues
 * @param cmd_path Path being checked
 * @param cmd_name Original command name
 * @return -1 if there was an error, status code otherwise
 */
int	handle_path_errors(char *cmd_path, char *cmd_name)
{
	if (is_directory(cmd_path))
	{
		ft_cmd_error(cmd_name, "is a directory", 126);
		free(cmd_path);
		return (-1);
	}
	if (access(cmd_path, F_OK) == 0 && access(cmd_path, X_OK) != 0)
	{
		ft_cmd_error(cmd_name, "permission denied", 126);
		free(cmd_path);
		return (-1);
	}
	return (0);
}

/**
 * Enhanced version of get_command_path that handles more edge cases
 * @param cmd Command name or path
 * @param env Environment variables
 * @return Full path to command or NULL if not found
 */
char	*get_command_path_enhanced(char *cmd, t_env *env)
{
	char	*cmd_path;
	char	**paths;
	int		i;

	if (!cmd || cmd[0] == '\0')
		return (NULL);

	// Handle case where command is a full path
	if (is_full_path(cmd))
	{
		cmd_path = ft_strdup(cmd);
		if (!cmd_path)
			return (NULL);

		// Check if file exists and has proper permissions
		if (handle_path_errors(cmd_path, cmd) == -1)
			return (NULL);

		// Check if file exists
		if (access(cmd_path, F_OK) != 0)
		{
			ft_cmd_error(cmd, "No such file or directory", 127);
			free(cmd_path);
			return (NULL);
		}
		return (cmd_path);
	}

	// Try to find command in PATH environment variable
	paths = split_paths(env);
	if (!paths)
		return (NULL);

	cmd_path = find_command_path(paths, cmd);

	// Free the paths array
	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);

	// If command not found in PATH
	if (!cmd_path)
	{
		ft_cmd_error(cmd, "command not found", 127);
		return (NULL);
	}

	return (cmd_path);
}

/**
 * Check if a command exists but is empty (empty file executable)
 * @param cmd_path Path to the command
 * @return true if file exists but is empty, false otherwise
 */
bool	is_empty_executable(char *cmd_path)
{
	struct stat	sb;

	if (!cmd_path || cmd_path[0] == '\0')
		return (false);

	if (stat(cmd_path, &sb) == -1)
		return (false);

	return (S_ISREG(sb.st_mode) && sb.st_size == 0 && (sb.st_mode & S_IXUSR));
}
