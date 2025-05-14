/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 14:32:48 by riel-fas          #+#    #+#             */
/*   Updated: 2024/11/14 16:29:27 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	len;
	size_t	x;

	len = ft_strlen(src);
	x = 0;
	if (!dstsize)
		return (len);
	while (src[x] != '\0' && x < dstsize - 1)
	{
		dst[x] = src[x];
		x++;
	}
	dst[x] = '\0';
	return (len);
}
