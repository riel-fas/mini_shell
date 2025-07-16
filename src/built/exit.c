/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:32:50 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:32:57 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

static int	is_digit(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	print_and_exit(t_list **env, t_shell *shell)
{
	shell->exit_status = 255;
	write(2, EXIT_ERROR, 32);
	ft_lstclear(env, free);
	exit(shell->exit_status);
}

int	exit_built(char **cmd, t_list **env, t_shell *shell)
{
	int	status;

	if (cmd[1] && cmd[2])
	{
		shell->exit_status = 1;
		return (write(2, "exit: too many arguments\n", 25), shell->exit_status);
	}
	else if (cmd[1])
	{
		if (is_digit(cmd[1]) == 0)
			print_and_exit(env, shell);
		status = ft_atoi(cmd[1]);
		if (status < 0)
			status = 256 + status;
		if (status > 255)
			status = status % 256;
		shell->exit_status = status;
	}
	else
		shell->exit_status = 0;
	ft_lstclear(env, free);
	exit(shell->exit_status);
}
