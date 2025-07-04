/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 21:36:55 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 22:10:53 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

#define BUFFER_SIZE 1000

char	*get_line(int fd, char *hold)
{
	char	buff[BUFFER_SIZE + 1];
	int		rd;

	rd = 1;
	while (rd > 0)
	{
		rd = read_and_append(fd, buff, &hold);
		if (rd < 0)
			return (NULL);
		if (!hold || my_strchr(hold, '\n'))
			break ;
	}
	return (hold);
}

int	next_line_index(char *hold)
{
	int	i;

	i = 0;
	while (hold[i] && hold[i] != '\n')
		i++;
	if (hold[i] == '\n')
		i++;
	return (i);
}

char	*extract_line(char **hold)
{
	char	*line;
	char	*temp;
	int		i;

	if (!*hold || !**hold)
		return (NULL);
	i = next_line_index(*hold);
	line = my_substr(*hold, 0, i);
	temp = ft_strdup(*hold + i);
	free(*hold);
	*hold = temp;
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*hold = NULL;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		free(hold);
		hold = NULL;
		return (NULL);
	}
	hold = get_line(fd, hold);
	if (!hold)
		return (NULL);
	line = extract_line(&hold);
	if (!line)
	{
		free(hold);
		hold = NULL;
		return (NULL);
	}
	return (line);
}
