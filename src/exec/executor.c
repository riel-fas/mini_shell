/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:45:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/03 01:10:22 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

extern int	g_exit_status;

/*
 * Counts the number of commands in the command list
 * Returns the total number of commands to execute
 */
static int	count_commands(t_cmds *commands)
{
	t_cmds	*current;
	int		count;

	count = 0;
	current = commands;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/*
 * Checks if a command has arguments (is not empty)
 * Returns 1 if command has arguments, 0 otherwise
 */
static int	has_command(t_cmds *cmd)
{
	return (cmd && cmd->args && cmd->args[0]);
}

/*
 * Main execution entry point - determines execution strategy
 * Handles single commands, pipelines, and redirection-only commands
 */
int	execute_commands(t_shell *shell, t_cmds *commands)
{
	int	num_cmds;
	int	status;

	if (!commands)
		return (0);

	num_cmds = count_commands(commands);

	if (num_cmds == 0)
		return (0);
	else if (num_cmds == 1 && !has_command(commands))
	{
		// Handle redirection-only commands
		status = handle_empty_command_redirections(commands);
		if (status == 130) // SIGINT during heredoc
			return (130);
		return (status);
	}
	else if (num_cmds == 1)
	{
		// Single command execution
		return (execute_single_command(shell, commands));
	}
	else
	{
		// Pipeline execution
		return (execute_pipeline(shell, commands));
	}
}
