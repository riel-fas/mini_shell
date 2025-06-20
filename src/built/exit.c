/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:15:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/13 20:43:53 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	is_numeric(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args[count])
		count++;
	return (count);
}

static int	handle_exit_error(char **args)
{
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		return (255);
	}
	return (-1);
}

int	builtin_exit(t_shell *shell, char **args)
{
	int	status;
	int	arg_count;

	ft_putendl_fd("exit", 2);
	arg_count = count_args(args);
	if (arg_count == 1)
		status = shell->exit_status;
	else if (arg_count > 2)
	{
		ft_putendl_fd("exit: too many arguments", 2);
		return (1);
	}
	else
	{
		status = handle_exit_error(args);
		if (status == -1)
			status = ft_atoi(args[1]) & 255;
	}
	cleanup(shell);
	exit(status);
	return (status);
}
