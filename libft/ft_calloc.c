/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 18:00:11 by riel-fas          #+#    #+#             */
/*   Updated: 2025/05/19 17:15:03 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdint.h>

void	*ft_calloc(size_t count, size_t size)
{
	char	*x;

	if (count != 0 && size > SIZE_MAX / count)
		return (NULL);
	x = (char *)malloc(count * size);
	if (!x)
		return (NULL);
	ft_bzero(x, count * size);
	return (x);
}
