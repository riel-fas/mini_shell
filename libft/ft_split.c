/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:24:58 by riel-fas          #+#    #+#             */
/*   Updated: 2024/11/15 11:44:12 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	countwords(const char *s, char c)
{
	size_t	index;
	size_t	count;

	index = 0;
	count = 0;
	while (s[index])
	{
		if (s[index] != c && (index == 0 || s[index - 1] == c))
			count++;
		index++;
	}
	return (count);
}

static char	**freeall(char **p, size_t x)
{
	while (x > 0)
		free (p[--x]);
	free (p);
	return (NULL);
}

static char	**copywords(char **p, const char *s, char c)
{
	size_t	i;
	size_t	start;
	size_t	x;

	i = 0;
	x = 0;
	while (s[i] && x < countwords (s, c))
	{
		while (s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		p[x++] = ft_substr(s, start, i - start);
		if (!p[x - 1])
			return (freeall(p, x));
	}
	p[x] = NULL;
	return (p);
}

char	**ft_split(const char *s, char c)
{
	size_t	count;
	char	**p;

	if (!s)
		return (NULL);
	count = countwords(s, c);
	p = (char **)malloc(sizeof(char *) * (count + 1));
	if (!p)
		return (NULL);
	return (copywords(p, s, c));
}
