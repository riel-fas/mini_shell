/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   Created: 2025/01/03 10:00:00 by user              #+#    #+#             */
/*   Updated: 2025/01/03 10:00:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

#define BUFFER_SIZE 1000

static char	*my_strchr(char *s, char c)
{
	int	i;

	if (!s)
		return (NULL);
	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (&s[i]);
		i++;
	}
	if (c == '\0')
		return (&s[i]);
	return (NULL);
}

static char	*my_strjoin(char *s1, char *s2)
{
	char	*result;
	int		i;
	int		j;
	int		len1;
	int		len2;

	if (!s1 && !s2)
		return (NULL);
	len1 = s1 ? ft_strlen(s1) : 0;
	len2 = s2 ? ft_strlen(s2) : 0;
	result = malloc(len1 + len2 + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (s1 && s1[j])
		result[i++] = s1[j++];
	j = 0;
	while (s2 && s2[j])
		result[i++] = s2[j++];
	result[i] = '\0';
	free(s1);
	return (result);
}

static char	*my_substr(char *s, unsigned int start, size_t len)
{
	char	*result;
	size_t	i;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	if (len > s_len - start)
		len = s_len - start;
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		result[i] = s[start + i];
		i++;
	}
	result[i] = '\0';
	return (result);
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
			return (NULL);
		}
		buff[rd] = '\0';
		hold = my_strjoin(hold, buff);
		if (!hold || my_strchr(hold, '\n'))
			break ;
	}
	return (hold);
}

static char	*extract_line(char **hold)
{
	char	*line;
	char	*temp;
	int		i;

	if (!*hold || !**hold)
		return (NULL);
	i = 0;
	while ((*hold)[i] && (*hold)[i] != '\n')
		i++;
	if ((*hold)[i] == '\n')
		i++;
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
