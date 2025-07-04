/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 02:50:26 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/04 02:50:26 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

int	open_input_file(char *file)
{
	int	fd;

	if (!file)
		return (-1);
	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		ft_cmd_error(file, "No such file or directory\n", 1);
		return (-1);
	}
	return (fd);
}

int	open_output_file(char *file, int append)
{
	int	fd;
	int	flags;

	if (!file)
		return (-1);
	if (append)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(file, flags, 0644);
	if (fd < 0)
	{
		ft_cmd_error(file, "Permission denied\n", 1);
		return (-1);
	}
	return (fd);
}

void	reset_redirections(int stdin_backup, int stdout_backup)
{
	if (stdin_backup >= 0)
	{
		dup2(stdin_backup, STDIN_FILENO);
		close(stdin_backup);
	}
	if (stdout_backup >= 0)
	{
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdout_backup);
	}
}

int	handle_input_redirection(t_cmds *cmd)
{
	int	fd;

	if (!cmd->input_file)
		return (0);
	fd = open_input_file(cmd->input_file);
	if (fd < 0)
		return (1);
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	handle_output_redirection(t_cmds *cmd)
{
	int	fd;

	if (!cmd->output_file)
		return (0);
	fd = open_output_file(cmd->output_file, cmd->append_node);
	if (fd < 0)
		return (1);
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}
