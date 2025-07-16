/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:45:48 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:45:54 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

bool	ambiguous_error(char *file)
{
	char	*tmp;

	if (!file || file[0] == '\0')
		return (true);
	tmp = file;
	while (*tmp)
	{
		if (*tmp == ' ')
			break ;
		tmp++;
	}
	if (*tmp == ' ')
		return (true);
	return (false);
}

void	finish_exec(pid_t last_cmd_pid, t_shell *shell)
{
	int	status;

	setup_parent_waiting_signals();
	wait_for_children(&status, last_cmd_pid, shell);
	restore_signals();
}
