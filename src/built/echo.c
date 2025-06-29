/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:15:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/29 02:58:11 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"

static int	is_n_flag(char *arg)
{
	int	j;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	j = 2;
	while (arg[j])
	{
		if (arg[j] != 'n')
			return (0);
		j++;
	}
	return (1);
}

int	builtin_echo(t_shell *shell, char **args)
{
	int	i;
	int	n_flag;

	(void)shell;
	n_flag = 0;
	i = 1;

	// Handle multiple -n flags (including -nn, -nnn, etc.)
	while (args[i] && is_n_flag(args[i]))
	{
		n_flag = 1;
		i++;
	}

	// Print remaining arguments
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}

	if (!n_flag)
		ft_putchar_fd('\n', 1);
	
	return (0);
}
