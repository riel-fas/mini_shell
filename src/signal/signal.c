/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:02:11 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:04:26 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

static t_shell	*g_sig_shell = NULL;

void	set_signal_shell(t_shell *shell)
{
	g_sig_shell = shell;
}

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	if (g_sig_shell)
		g_sig_shell->exit_status = 1;
}

void	handle_parent_sigint(int sig)
{
	(void)sig;
	if (g_sig_shell)
		g_sig_shell->exit_status = 130;
}

void	handle_parent_heredoc_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	if (g_sig_shell)
		g_sig_shell->exit_status = 1;
}
