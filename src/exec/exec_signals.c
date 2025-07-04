/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_signals.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 03:04:14 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 03:45:52 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

void	handle_child_sigint(int sig)
{
	(void)sig;
	exit(130);
}

void	handle_child_sigquit(int sig)
{
	(void)sig;
	ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
	exit(131);
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	handle_parent_sigint(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', STDERR_FILENO);
}

void	setup_parent_waiting_signals(void)
{
	signal(SIGINT, handle_parent_sigint);
	signal(SIGQUIT, SIG_IGN);
}
