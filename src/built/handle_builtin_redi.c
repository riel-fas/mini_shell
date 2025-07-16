/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtin_redi.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:34:43 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:34:49 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

int	open_append(t_redi *output, t_shell *shell)
{
	output->tmp_fd = redi_for_builtin(output->file, 2, shell);
	if (output->tmp_fd == -1)
		return (0);
	if (output->is_last)
	{
		dup2(output->tmp_fd, STDOUT_FILENO);
		close(output->tmp_fd);
	}
	else
		close(output->tmp_fd);
	return (1);
}

int	open_output(t_redi *output, t_shell *shell)
{
	if (output->type == OUTPUT)
	{
		output->tmp_fd = redi_for_builtin(output->file, 0, shell);
		if (output->tmp_fd == -1)
			return (-1);
		if (output->is_last)
		{
			dup2(output->tmp_fd, STDOUT_FILENO);
			close(output->tmp_fd);
		}
		close(output->tmp_fd);
	}
	if (output->type == APPEND)
		if (!open_append(output, shell))
			return (-1);
	output = output->next;
	return (1);
}

int	open_input(t_redi *input, t_shell *shell)
{
	int	in_fd;

	if (input->type == INPUT)
	{
		in_fd = redi_for_builtin(input->file, 1, shell);
		if (in_fd == -1)
			return (-1);
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	input = input->next;
	return (1);
}

int	open_builtin_redi(t_cmd *cmdarg_list, t_shell *shell)
{
	if (cmdarg_list == NULL || cmdarg_list->redirections == NULL)
		return (1);
	cmdarg_list->origin_stdin = dup(STDIN_FILENO);
	if (cmdarg_list->origin_stdin == -1)
		return (perror("dup"), shell->exit_status = 1, 1);
	cmdarg_list->origin_stdout = dup(STDOUT_FILENO);
	if (cmdarg_list->origin_stdout == -1)
		return (perror("dup"), shell->exit_status = 1, 1);
	if (open_redirections(cmdarg_list->redirections, shell) == -1)
		return (1);
	return (0);
}

int	check_builtin(t_cmd *cmdarg_list, t_list **minienv, t_shell *shell)
{
	int	check;

	if (cmdarg_list == NULL)
		return (1);
	if (cmdarg_list->cmd == NULL)
		return (0);
	check = is_builtin(cmdarg_list->cmd[0]);
	if (size_list(cmdarg_list) == 1 && !check)
	{
		if (cmdarg_list->redirections
			&& open_builtin_redi(cmdarg_list, shell) == 1)
		{
			if (cmdarg_list->redirections)
				std_resetting(cmdarg_list);
			return (1);
		}
		if (run(cmdarg_list, minienv, shell) == 1)
		{
			if (cmdarg_list->redirections)
				std_resetting(cmdarg_list);
			return (1);
		}
	}
	return (0);
}
