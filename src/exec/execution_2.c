/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:46:46 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:46:52 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

int	create_pipe(int *pip, t_shell *shell)
{
	int	p;

	p = pipe(pip);
	if (p == -1)
	{
		command_error(NULL, "pipe error\n", 1);
		shell->exit_status = 1;
		return (0);
	}
	return (1);
}

void	parent_processing(int *tmp_in, int *pip_fd, t_cmd *current_cmd)
{
	if (*tmp_in != 0)
		close(*tmp_in);
	if (current_cmd->next)
	{
		close(pip_fd[1]);
		*tmp_in = pip_fd[0];
	}
	pipe_closed(current_cmd->input);
}
