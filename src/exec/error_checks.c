/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 13:00:00 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 02:40:59 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"
#include "../../includes/executor.h"

static int	check_output_file_errors(char *output_file)
{
	struct stat	sb;

	if (access(output_file, F_OK) == 0 && access(output_file, W_OK) != 0)
	{
		ft_cmd_error(output_file, "Permission denied", 1);
		return (1);
	}
	if (stat(output_file, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		ft_cmd_error(output_file, "Is a directory", 1);
		return (1);
	}
	return (0);
}

static int	check_rw_file_errors(char *rw_file)
{
	struct stat	sb;

	if (access(rw_file, F_OK) == 0
		&& (access(rw_file, R_OK) != 0 || access(rw_file, W_OK) != 0))
	{
		ft_cmd_error(rw_file, "Permission denied", 1);
		return (1);
	}
	if (stat(rw_file, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		ft_cmd_error(rw_file, "Is a directory", 1);
		return (1);
	}
	return (0);
}

int	check_redirection_errors(t_cmds *cmd)
{
	if (cmd->input_file && access(cmd->input_file, F_OK) != 0)
	{
		ft_cmd_error(cmd->input_file, "No such file or directory", 1);
		return (1);
	}
	if (cmd->input_file && access(cmd->input_file, R_OK) != 0)
	{
		ft_cmd_error(cmd->input_file, "Permission denied", 1);
		return (1);
	}
	if (cmd->output_file)
	{
		if (check_output_file_errors(cmd->output_file))
			return (1);
	}
	if (cmd->rw_file)
	{
		if (check_rw_file_errors(cmd->rw_file))
			return (1);
	}
	return (0);
}

int	check_pipeline_errors(t_cmds *commands, t_env *env)
{
	t_cmds	*current;
	int		error_code;

	current = commands;
	while (current)
	{
		if (current->args && current->args[0])
		{
			error_code = check_command_errors(current->args[0], env);
			if (error_code != 0)
				return (error_code);
		}
		if (check_redirection_errors(current))
			return (1);
		current = current->next;
	}
	return (0);
}
