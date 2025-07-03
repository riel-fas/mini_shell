/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   advanced_cmd_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 14:30:00 by riad              #+#    #+#             */
/*   Updated: 2025/07/03 14:30:00 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

/*
 * Advanced command path resolution with comprehensive error checking
 * Handles absolute paths, relative paths, and PATH environment variable
 */
char	*get_command_path(char *cmd, t_env *env)
{
	char		*path_env;
	char		**paths;
	char		*full_path;
	char		*temp;
	int			i;
	struct stat	file_stat;

	if (!cmd || !*cmd)
		return (NULL);

	// Handle absolute and relative paths
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) != 0)
			return (NULL);

		if (stat(cmd, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
			return (NULL); // Directory, not executable

		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));

		return (NULL); // File exists but not executable
	}

	// Search in PATH
	path_env = get_env_value(env, "PATH");
	if (!path_env)
		return (NULL);

	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
		{
			free_env_array(paths);
			return (NULL);
		}

		full_path = ft_strjoin(temp, cmd);
		free(temp);

		if (!full_path)
		{
			free_env_array(paths);
			return (NULL);
		}

		if (access(full_path, F_OK) == 0)
		{
			// Check if it's a directory
			if (stat(full_path, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
			{
				free(full_path);
				i++;
				continue;
			}

			// Check execute permission
			if (access(full_path, X_OK) == 0)
			{
				free_env_array(paths);
				return (full_path);
			}
		}

		free(full_path);
		i++;
	}

	free_env_array(paths);
	return (NULL);
}

/*
 * Checks command access and returns appropriate status
 * Returns: 0 = executable, 1 = permission denied, 2 = not found, 3 = directory
 */
int	check_command_access(char *path)
{
	struct stat	file_stat;

	if (!path)
		return (2);

	if (access(path, F_OK) != 0)
		return (2); // File not found

	if (stat(path, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
		return (3); // Is a directory

	if (access(path, X_OK) != 0)
		return (1); // Permission denied

	return (0); // Executable
}

/*
 * Converts environment linked list to array format for execve
 * Returns NULL-terminated array of "KEY=VALUE" strings
 */
char	**convert_env_to_array(t_env *env)
{
	t_env	*current;
	char	**env_array;
	char	*temp;
	int		count;
	int		i;

	// Count environment variables
	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}

	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);

	current = env;
	i = 0;
	while (current)
	{
		temp = ft_strjoin(current->key, "=");
		if (!temp)
		{
			free_env_array(env_array);
			return (NULL);
		}

		if (current->value)
			env_array[i] = ft_strjoin(temp, current->value);
		else
			env_array[i] = ft_strjoin(temp, "");

		free(temp);

		if (!env_array[i])
		{
			free_env_array(env_array);
			return (NULL);
		}

		current = current->next;
		i++;
	}

	env_array[i] = NULL;
	return (env_array);
}

/*
 * Enhanced error handling for command execution
 * Provides specific error messages based on the type of failure
 */
void	handle_execution_error(char *cmd_path, char *cmd_name)
{
	int	access_status;

	access_status = check_command_access(cmd_path);

	if (access_status == 1)
	{
		ft_cmd_error(cmd_name, "Permission denied", 126);
	}
	else if (access_status == 2)
	{
		ft_cmd_error(cmd_name, "No such file or directory", 127);
	}
	else if (access_status == 3)
	{
		ft_cmd_error(cmd_name, "Is a directory", 126);
	}
	else
	{
		ft_cmd_error(cmd_name, "Execution failed", 1);
	}
}

/*
 * Enhanced error message function with proper formatting
 * Prints error messages in the format: "minishell: cmd: error"
 */
void	ft_cmd_error(char *cmd, char *error, int status)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (cmd)
	{
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(error, STDERR_FILENO);

	// Update global exit status if provided
	if (status >= 0)
		g_exit_status = status;
}

/*
 * Frees a NULL-terminated array of strings
 * Used for cleaning up environment arrays and path arrays
 */
void	free_env_array(char **env_array)
{
	int	i;

	if (!env_array)
		return;

	i = 0;
	while (env_array[i])
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}
