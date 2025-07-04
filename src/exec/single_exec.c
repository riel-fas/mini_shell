/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 00:36:35 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 03:48:53 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

int	execute_single_command_advanced(t_shell *shell, t_cmds *cmd)
{
	if (!cmd->args || !cmd->args[0])
		return (handle_empty_command_redirections(cmd));
	if (ft_strcmp(cmd->args[0], "!") == 0 && !cmd->args[1])
		return (1);
	if (is_builtin(cmd->args[0]))
		return (execute_builtin_with_redirections(shell, cmd));
	return (execute_external_command(shell, cmd));
}

static int	has_redirections(t_cmds *cmd)
{
	if (cmd->input_file || cmd->output_file || cmd->rw_file)
		return (1);
	if (cmd->heredoc_delimeter || cmd->heredoc_fd >= 0 || cmd->heredoc_list)
		return (1);
	return (0);
}

int	handle_empty_command_redirections(t_cmds *cmd)
{
	int	stdin_backup;
	int	stdout_backup;
	int	status;
	int	devnull;

	if (!has_redirections(cmd))
		return (0);
	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	devnull = open("/dev/null", O_WRONLY);
	if (devnull >= 0)
	{
		dup2(devnull, STDOUT_FILENO);
		close(devnull);
	}
	status = setup_redirections(cmd);
	reset_redirections(stdin_backup, stdout_backup);
	if (status == 130)
		return (130);
	return (status);
}

int	execute_single_command(t_shell *shell, t_cmds *cmd)
{
	if (!cmd)
		return (0);
	return (execute_single_command_advanced(shell, cmd));
}
