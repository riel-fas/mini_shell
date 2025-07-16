/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:41:44 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:42:03 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

int	count(char *s, char p)
{
	int	i;

	i = 0;
	while (*s)
	{
		while (*s && *s == p)
			s++;
		if (!s)
			break ;
		if (*s && *s == '{')
		{
			while (*s && *s != '}')
				s++;
		}
		while (*s && !(*s == p || *s == '{'))
			s++;
		while (*s && *s == p)
			s++;
		if (!s)
			break ;
		i++;
	}
	return (i);
}

void	skip(char **s, int *len, char p, char **start)
{
	char	*end;

	while (**s && **s == p)
		(*s)++;
	if (!*s)
		return ;
	*start = *s;
	if (**s && **s == '{')
	{
		*start = *s;
		while (**s && **s != '}')
			(*s)++;
		(*s)++;
		end = *s;
	}
	else
	{
		while (**s && !(**s == p))
			(*s)++;
		end = *s;
	}
	*len = end - *start;
}

void	edit(int len, int j, char *start, char **buff)
{
	int	i;

	i = 0;
	while (i < len)
		buff[j][i++] = *start++;
	buff[j][i] = '\0';
}

char	**parsing_split(char *s, char p)
{
	int		wc;
	int		len;
	int		j;
	char	**buff;
	char	*start;

	if (!s)
		return (NULL);
	wc = count(s, p);
	len = 0;
	buff = malloc((wc + 1) * sizeof(char *));
	if (!buff)
		return (NULL);
	j = 0;
	while (j < wc)
	{
		skip(&s, &len, p, &start);
		buff[j] = malloc(len + 1);
		if (!buff[j])
			return (free_all(buff, j - 1), NULL);
		edit(len, j, start, buff);
		j++;
	}
	return (buff[j] = NULL, buff);
}
