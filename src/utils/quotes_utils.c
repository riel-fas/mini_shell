/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 23:57:28 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/15 23:57:36 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

int	skip_char(char *input, int i)
{
	while (input[i] && input[i] != '"' && input[i] != '\'')
		i++;
	return (i);
}

bool	quotes_scan(char *input, t_shell *shell)
{
	int		i;
	char	q;

	i = 0;
	while (input[i])
	{
		i = skip_char(input, i);
		if (input[i] == '"' || input[i] == '\'')
		{
			q = input[i];
			i++;
			while (input[i] && input[i] != q)
				i++;
			if (!input[i])
				return (printf(UNCLOSED), shell->exit_status = 258, false);
			if (input[i])
				i++;
		}
		else if (input[i])
			i++;
	}
	return (true);
}
