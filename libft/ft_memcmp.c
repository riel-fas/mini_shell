/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 15:45:38 by riel-fas          #+#    #+#             */
/*   Updated: 2024/11/15 12:15:55 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*ps1;
	const unsigned char	*ps2;
	size_t				x;

	x = 0;
	ps1 = s1;
	ps2 = s2;
	while (x < n)
	{
		if (ps1[x] != ps2[x])
			return (ps1[x] - ps2[x]);
		x++;
	}
	return (0);
}
