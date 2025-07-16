/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_run_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:44:27 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:44:35 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

void	printing_exit(t_cmd *current_cmd, int no_file)
{
	if (no_file == 1)
		command_error(current_cmd->cmd[0], "No such file or directory\n", 127);
	else
		command_error(current_cmd->cmd[0], "command not found\n", 127);
}

void	handle_heredoc(t_redi *input)
{
	if (dup2(input->heredoc_fd, STDIN_FILENO) == -1)
		command_error(NULL, "----------- dup2 failure\n", 1);
	close(input->heredoc_fd);
}

int	handel_append(t_redi *output)
{
	int	out_fd;

	if (output->variable)
		if (ambiguous_error(output->file) == true)
			command_error(output->file, "ambiguous redirect\n", 1);
	out_fd = open_file(output->file, 2);
	if (output->is_last)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
		{
			close(out_fd);
			command_error(NULL, "dup2 failure\n", 1);
		}
		close(out_fd);
	}
	return (1);
}

void	handle_output(t_redi *output)
{
	int	out_fd;

	if (output->type == OUTPUT)
	{
		if (output->variable)
			if (ambiguous_error(output->file) == true)
				command_error(output->file, "ambiguous redirect\n", 1);
		out_fd = open_file(output->file, 0);
		if (dup2(out_fd, STDOUT_FILENO) == -1)
		{
			close(out_fd);
			command_error(NULL, "dup2 failure\n", 1);
		}
		close(out_fd);
	}
	if (output->type == APPEND)
		handel_append(output);
	output = output->next;
}

void	handle_input(t_redi *input)
{
	int	in_fd;

	if (input->type == INPUT)
	{
		if (input->variable)
			if (ambiguous_error(input->file) == true)
				command_error(input->file, "ambiguous redirect\n", 1);
		in_fd = open_file(input->file, 1);
		if (dup2(in_fd, STDIN_FILENO) == -1)
		{
			close(in_fd);
			command_error(NULL, "dup2 failure\n", 1);
		}
		close(in_fd);
	}
	if (input->type == HEREDOC && input->is_last == true)
	{
		handle_heredoc(input);
	}
	input = input->next;
}
