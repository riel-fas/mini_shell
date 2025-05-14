/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:12:30 by riel-fas          #+#    #+#             */
/*   Updated: 2024/11/16 13:56:05 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	lend;
	size_t	lens;
	size_t	i;
	size_t	j;

	if (dst == NULL && dstsize == 0)
		return (ft_strlen(src));
	lend = ft_strlen(dst);
	lens = ft_strlen(src);
	if (dstsize == 0)
		return (lens);
	if (dstsize <= lend)
		return (dstsize + lens);
	i = lend;
	j = 0;
	while (src[j] && (i < dstsize - 1))
		dst[i++] = src[j++];
	dst[i] = '\0';
	return (lend + lens);
}
