/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:39:54 by riel-fas          #+#    #+#             */
/*   Updated: 2024/11/14 14:07:31 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*temp;
	char	cc;

	temp = (char *)s;
	cc = (char)c;
	while (*temp)
	{
		if (*temp == cc)
			return (temp);
		temp++;
	}
	if (cc == '\0')
		return (temp);
	return (0);
}
