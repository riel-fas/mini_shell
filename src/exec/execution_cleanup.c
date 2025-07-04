/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_cleanup.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 03:09:01 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/04 03:09:01 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

static int	get_process_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			return (130);
		else if (WTERMSIG(status) == SIGQUIT)
			return (131);
		else
			return (128 + WTERMSIG(status));
	}
	return (0);
}

void	finish_execution(pid_t last_pid)
{
	int		status;
	pid_t	wpid;
	int		last_status;

	last_status = 0;
	setup_parent_waiting_signals();
	wpid = wait(&status);
	while (wpid > 0)
	{
		if (wpid == last_pid)
			last_status = get_process_exit_status(status);
	}
	restore_signals();
	g_state.exit_status = last_status;
}
