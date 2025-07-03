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

static int	has_command(t_cmds *cmd)
{
	return (cmd && cmd->args && cmd->args[0]);
}

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
		if (commands->input_file || commands->output_file || commands->rw_file || commands->heredoc_delimeter)
		{
			int stdin_backup = dup(STDIN_FILENO);
			int stdout_backup = dup(STDOUT_FILENO);

			// Redirect stdout to /dev/null to suppress output from heredocs
			int devnull = open("/dev/null", O_WRONLY);
			if (devnull >= 0)
			{
				dup2(devnull, STDOUT_FILENO);
				close(devnull);
			}

			status = setup_redirections(commands);
			// Just consume the redirections but don't produce any output for standalone redirs
			reset_redirections(stdin_backup, stdout_backup);

			if (status == 130) // If interrupted by SIGINT, return 130 but don't exit shell
				return (130);
			return (status);
		}
		return (0);
	}
	else if (num_cmds == 1)
		return (execute_single_command(shell, commands));
	else
		return (execute_pipeline(shell, commands));
}
