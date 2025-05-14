/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:22:30 by riel-fas          #+#    #+#             */
/*   Updated: 2024/11/14 15:41:06 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			x;
	unsigned char	*p;

	x = 0;
	p = (unsigned char *)s;
	while (x < n)
	{
		if (p[x] == (unsigned char )c)
			return ((void *)&p[x]);
		x++;
	}
	return (0);
}
