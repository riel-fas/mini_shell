/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 16:24:53 by riel-fas          #+#    #+#             */
/*   Updated: 2024/11/15 08:39:36 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *small, size_t len)
{
	size_t	x;
	size_t	j;

	x = 0;
	if (big == NULL && len == 0 && small)
		return (NULL);
	if (small[x] == '\0')
		return ((char *)big);
	while (big[x] && x < len)
	{
		j = 0;
		while (big[x + j] == small[j] && small[j] != '\0' && x + j < len)
			j++;
		if (small[j] == '\0')
			return ((char *)big + x);
		x++;
	}
	return (NULL);
}
