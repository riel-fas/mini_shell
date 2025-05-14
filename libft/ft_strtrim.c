/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 12:01:10 by riel-fas          #+#    #+#             */
/*   Updated: 2024/11/10 11:58:17 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check(char const *s1, char set)
{
	int	x;

	x = 0;
	while (s1[x] != '\0')
	{
		if (s1[x] == set)
			return (1);
		x++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	len;
	size_t	a;
	char	*sub;

	if (!s1)
		return (NULL);
	if (!set)
		return ((char *)s1);
	len = ft_strlen(s1) - 1;
	a = 0;
	while (s1[a] != '\0' && check(set, s1[a]))
		a++;
	while (check(set, s1[len]) && s1[len])
		len--;
	sub = ft_substr(s1, a, len - a + 1);
	return (sub);
}
