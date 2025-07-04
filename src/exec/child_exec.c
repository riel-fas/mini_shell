/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 00:36:35 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 03:44:54 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

static int	check_directory_path(char *cmd_path)
{
	struct stat	file_stat;

	if (ft_strchr(cmd_path, '/'))
	{
		if (access(cmd_path, F_OK) == 0
			&& stat(cmd_path, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
		{
			ft_cmd_error(cmd_path, "Is a directory", 126);
			exit(126);
		}
	}
	return (0);
}

static void	cleanup_and_exit(char *cmd_path, char **env_array, int exit_code)
{
	if (cmd_path)
		free(cmd_path);
	if (env_array)
		free_env_array(env_array);
	exit(exit_code);
}

void	execute_child_command(t_shell *shell, t_cmds *cmd)
{
	char	*cmd_path;
	char	**env_array;

	setup_child_signals();
	check_directory_path(cmd->args[0]);
	cmd_path = get_command_path(cmd->args[0], shell->env);
	if (!cmd_path)
	{
		ft_cmd_error(cmd->args[0], "command not found", 127);
		exit(127);
	}
	env_array = convert_env_to_array(shell->env);
	if (!env_array)
	{
		free(cmd_path);
		ft_cmd_error(cmd->args[0], "Failed to create environment", 1);
		exit(1);
	}
	execve(cmd_path, cmd->args, env_array);
	handle_execution_error(cmd_path, cmd->args[0]);
	cleanup_and_exit(cmd_path, env_array, 1);
}
