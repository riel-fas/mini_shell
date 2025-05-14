/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 09:24:45 by riel-fas          #+#    #+#             */
/*   Updated: 2024/11/14 17:13:04 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*copy;
	size_t	y;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	else if (start + len > ft_strlen(s))
		len = ft_strlen(s) - start;
	copy = (char *)malloc(len + 1);
	if (!copy)
		return (NULL);
	y = 0;
	while (s[start + y] && y < len)
	{
		copy[y] = s[start + y];
		y++;
	}
	copy[y] = '\0';
	return (copy);
}
