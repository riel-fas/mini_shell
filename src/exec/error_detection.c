/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_detection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 13:00:00 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 17:59:14 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"
#include "../../includes/executor.h"

/**
 * Check for common command errors
 * @param cmd Command to check
 * @param env Environment variables
 * @return 0 if no errors, error code otherwise
 */
int	check_command_errors(char *cmd, t_env *env)
{
	struct stat	sb;
	char		*cmd_path;

	if (!cmd || cmd[0] == '\0')
		return (0); // Empty command, will be handled elsewhere

	// Check if it's a built-in command
	if (is_builtin(cmd))
		return (0);

	// Check if absolute or relative path
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (stat(cmd, &sb) == -1)
		{
			ft_cmd_error(cmd, "No such file or directory", 127);
			return (127);
		}
		if (S_ISDIR(sb.st_mode))
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

	// Check PATH
	cmd_path = get_command_path_enhanced(cmd, env);
	if (!cmd_path)
		return (127);

	free(cmd_path);
	return (0);
}

/**
 * Check for errors in redirections
 * @param cmd Command structure with redirections
 * @return 0 if no errors, 1 if errors found
 */
int	check_redirection_errors(t_cmds *cmd)
{
	// Check input file
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

	// Check output file
	if (cmd->output_file)
	{
		if (access(cmd->output_file, F_OK) == 0 && access(cmd->output_file, W_OK) != 0)
		{
			ft_cmd_error(cmd->output_file, "Permission denied", 1);
			return (1);
		}

		// Check if it's a directory
		struct stat sb;
		if (stat(cmd->output_file, &sb) == 0 && S_ISDIR(sb.st_mode))
		{
			ft_cmd_error(cmd->output_file, "Is a directory", 1);
			return (1);
		}
	}

	// Check read-write file
	if (cmd->rw_file)
	{
		if (access(cmd->rw_file, F_OK) == 0 &&
			(access(cmd->rw_file, R_OK) != 0 || access(cmd->rw_file, W_OK) != 0))
		{
			ft_cmd_error(cmd->rw_file, "Permission denied", 1);
			return (1);
		}

		// Check if it's a directory
		struct stat sb;
		if (stat(cmd->rw_file, &sb) == 0 && S_ISDIR(sb.st_mode))
		{
			ft_cmd_error(cmd->rw_file, "Is a directory", 1);
			return (1);
		}
	}

	return (0);
}

/**
 * Check for errors in a pipeline before execution
 * @param commands List of commands in the pipeline
 * @param env Environment variables
 * @return 0 if no errors, error code otherwise
 */
int	check_pipeline_errors(t_cmds *commands, t_env *env)
{
	t_cmds	*current;
	int		error_code;

	current = commands;
	while (current)
	{
		// Skip command checks if it's just redirections with no command
		if (current->args && current->args[0])
		{
			error_code = check_command_errors(current->args[0], env);
			if (error_code != 0)
				return (error_code);
		}

		// Check redirection errors
		if (check_redirection_errors(current))
			return (1);

		current = current->next;
	}

	return (0);
}

/**
 * Comprehensive error detection before command execution
 * @param shell Shell structure
 * @param commands Commands to check
 * @return 0 if no errors detected, error code otherwise
 */
int	detect_execution_errors(t_shell *shell, t_cmds *commands)
{
	int	error_code;

	// Check for pipeline errors
	error_code = check_pipeline_errors(commands, shell->env);
	if (error_code != 0)
		return (error_code);

	return (0);
}
