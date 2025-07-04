/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_detection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 13:00:00 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 20:50:24 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"
#include "../../includes/executor.h"
#include "../../includes/error.h"

static int	check_file_permissions(char *cmd, struct stat *sb)
{
	if (stat(cmd, sb) == -1)
	{
		ft_cmd_error(cmd, "No such file or directory", 127);
		return (127);
	}
	if (S_ISDIR(sb->st_mode))
	{
		ft_cmd_error(cmd, "is a directory", 126);
		return (126);
	}
	if (access(cmd, X_OK) != 0)
	{
		ft_cmd_error(cmd, "Permission denied", 126);
		return (126);
	}
	return (0);
}

int	check_command_errors(char *cmd, t_env *env)
{
	struct stat	sb;
	char		*cmd_path;
	int			result;

	if (!cmd || cmd[0] == '\0')
		return (0);
	if (is_builtin(cmd))
		return (0);
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		result = check_file_permissions(cmd, &sb);
		return (result);
	}
	cmd_path = get_command_path(cmd, env);
	if (!cmd_path)
		return (127);
	free(cmd_path);
	return (0);
}

int	detect_execution_errors(t_shell *shell, t_cmds *commands)
{
	int	error_code;

	error_code = check_pipeline_errors(commands, shell->env);
	if (error_code != 0)
		return (error_code);
	return (0);
}
