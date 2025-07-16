/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:48:12 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 01:23:57 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

void	free_every_char(char *s)
{
	if (s)
	{
		free(s);
		s = NULL;
	}
}

void	command_error(char *cmd_name, char *error, int status)
{
	write(2, "minishell : ", 11);
	if (cmd_name)
	{
		write(2, cmd_name, ft_strlen(cmd_name));
		write(2, ": ", 2);
	}
	write(2, error, ft_strlen(error));
	exit(status);
}

int	open_file(char *file, int flag)
{
	int	fd;

	fd = 0;
	if (flag == 0)
		fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (flag == 1)
		fd = open(file, O_RDONLY);
	else
		fd = open(file, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		if (errno == ENOENT)
			command_error(file, "No such file or directory\n", 1);
		else if (errno == EISDIR)
			command_error(file, "Is a directory\n", 1);
		else
			command_error(file, "Permission denied\n", 1);
	}
	return (fd);
}

void	free_error_handling(char **cmd_path, char **cmd_name, char **envp)
{
	free(*cmd_path);
	*cmd_path = NULL;
	free_buitl(envp);
	*envp = NULL;
	free(*cmd_name);
	*cmd_name = NULL;
	if (errno == EACCES)
		command_error(NULL, "Permission denied\n", 126);
	else
		command_error(NULL, "execution failure\n", 1);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (-1);
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			break ;
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}
