/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtin_redi_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:34:18 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 01:28:18 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

int	redi_for_builtin(char *file, int flag, t_shell *shell)
{
	int	fd;

	fd = 0;
	if (file[0] == '\0')
		return (redi_errors(file, "No such file or directory\n", 1, shell), -1);
	if (ambiguous_error(file) == true)
		return (redi_errors(file, "ambiguous redirect\n", 1, shell), -1);
	if (flag == 0)
		fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (flag == 1)
		fd = open(file, O_RDONLY);
	else
		fd = open(file, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		if (errno == ENOENT)
			redi_errors(file, "No such file or directory\n", 1, shell);
		else if (errno == EISDIR)
			redi_errors(file, "Is a directory\n", 1, shell);
		else
			redi_errors(file, "Permission denied\n", 1, shell);
	}
	return (fd);
}

void	std_resetting(t_cmd *shell)
{
	if (dup2(shell->origin_stdout, STDOUT_FILENO) < 0)
	{
		write(2, "dup2 failure\n", 13);
		return ;
	}
	if (dup2(shell->origin_stdin, STDIN_FILENO) < 0)
	{
		write(2, "dup2 failure\n", 13);
		return ;
	}
	close(shell->origin_stdout);
	close(shell->origin_stdin);
	shell->origin_stdout = -1;
	shell->origin_stdin = -1;
	return ;
}

void	redi_errors(char *file, char *msg, int err, t_shell *shell)
{
	write(2, "minishell : ", 11);
	if (file)
	{
		write(2, file, ft_strlen(file));
		write(2, ": ", 2);
	}
	write(2, msg, ft_strlen(msg));
	shell->exit_status = err;
}

int	open_redirections(t_redi *redirections, t_shell *shell)
{
	t_redi	*redi;

	redi = redirections;
	if (redi == NULL)
		return (1);
	while (redi)
	{
		if (redi->type == INPUT)
		{
			if (open_input(redi, shell) == -1)
				return (-1);
		}
		if (redi->type == OUTPUT || redi->type == APPEND)
		{
			if (open_output(redi, shell) == -1)
				return (-1);
		}
		redi = redi->next;
	}
	return (1);
}

bool	heredoc_cant_exp(t_token *tmp)
{
	return ((tmp->type == WORD || tmp->type == DOUBLE_QUOTE)
		&& tmp->prev && tmp->prev->type == HEREDOC);
}
