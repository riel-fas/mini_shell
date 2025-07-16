/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:03:34 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:07:44 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*resit(char *hold)
{
	int		i;
	char	*ret;

	i = 0;
	if (!hold)
		return (NULL);
	while (hold[i])
	{
		if (hold[i] == '\n')
		{
			ret = my_substr(hold, i + 1, my_strlen(hold) - i);
			free(hold);
			hold = NULL;
			return (ret);
		}
		i++;
	}
	return (NULL);
}

char	*befor_newline(char **hold)
{
	int		i;
	char	*line;
	char	*current;

	i = 0;
	current = *hold;
	if (hold && current)
	{
		while (current[i])
		{
			if (current[i] == '\n')
				return (my_substr(current, 0, i + 1));
			i++;
		}
		if (current[i] == '\0')
		{
			line = my_substr(current, 0, i);
			free(current);
			*hold = NULL;
			return (line);
		}
	}
	return (NULL);
}

static char	*get_line(int fd, char *hold)
{
	char	buff[BUFFER_SIZE + 1];
	int		rd;

	rd = 1;
	while (rd > 0)
	{
		rd = read(fd, buff, BUFFER_SIZE);
		if (rd < 0)
		{
			free(hold);
			hold = NULL;
			return (NULL);
		}
		buff[rd] = '\0';
		hold = my_strjoin(hold, buff);
		if (!hold || my_strchr(hold, '\n'))
			break ;
	}
	return (hold);
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
	line = befor_newline(&hold);
	if (!line)
	{
		free(hold);
		hold = NULL;
		return (NULL);
	}
	hold = resit(hold);
	return (line);
}
