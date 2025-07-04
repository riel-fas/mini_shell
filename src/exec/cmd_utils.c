/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 00:36:10 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 03:45:17 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

int	check_command_access(char *path)
{
	struct stat	file_stat;

	if (!path)
		return (2);
	if (access(path, F_OK) != 0)
		return (2);
	if (stat(path, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
		return (3);
	if (access(path, X_OK) != 0)
		return (1);
	return (0);
}

void	handle_execution_error(char *cmd_path, char *cmd_name)
{
	int	access_status;

	access_status = check_command_access(cmd_path);
	if (access_status == 1)
		ft_cmd_error(cmd_name, "Permission denied", 126);
	else if (access_status == 2)
		ft_cmd_error(cmd_name, "No such file or directory", 127);
	else if (access_status == 3)
		ft_cmd_error(cmd_name, "Is a directory", 126);
	else
		ft_cmd_error(cmd_name, "Execution failed", 1);
}

void	ft_cmd_error(char *cmd, char *error, int status)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (cmd)
	{
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(error, STDERR_FILENO);
	if (status >= 0)
		g_state.exit_status = status;
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
