/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 14:30:00 by riad              #+#    #+#             */
/*   Updated: 2025/07/03 14:30:00 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

extern int	g_exit_status;

/*
 * Signal handler for child processes during execution
 * Allows default signal behavior for child processes
 */
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

/*
 * Signal handler for parent process while waiting for children
 * Ignores signals to let children handle them
 */
void	handle_parent_sigint(int sig)
{
	(void)sig;
	// Do nothing - let child handle the signal
	ft_putchar_fd('\n', STDERR_FILENO);
}

/*
 * Signal handler for parent during heredoc processing
 * Handles interruption of heredoc input collection
 */
void	handle_parent_heredoc_sigint(int sig)
{
	(void)sig;
	g_exit_status = 130;
	ft_putchar_fd('\n', STDERR_FILENO);
}

/*
 * Signal handler for heredoc input (Ctrl+C)
 * Exits child process with status 130
 */
void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(130);
}

/*
 * Sets up signal handlers for child processes
 * Allows child processes to be interrupted normally
 */
void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);   // Default behavior for SIGINT
	signal(SIGQUIT, SIG_DFL);  // Default behavior for SIGQUIT
}

/*
 * Sets up signal handlers for parent process during command execution
 * Parent ignores signals to let children handle them appropriately
 */
void	setup_parent_waiting_signals(void)
{
	signal(SIGINT, handle_parent_sigint);
	signal(SIGQUIT, SIG_IGN);
}

/*
 * Sets up signal handlers for heredoc processing
 * Special handling for heredoc input collection
 */
void	setup_heredoc_signals(void)
{
	signal(SIGINT, handle_heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
}

/*
 * Sets up signal handlers for parent during heredoc processing
 * Handles signals when parent is coordinating heredoc input
 */
void	setup_parent_heredoc_signals(void)
{
	signal(SIGINT, handle_parent_heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
}

/*
 * Restores signal handlers to their default shell behavior
 * Called after command execution is complete
 */
void	restore_signals(void)
{
	signal(SIGINT, handler);      // Restore main shell handler
	signal(SIGQUIT, sigquit_handler); // Restore main shell handler
}

/*
 * Restores signal handlers after heredoc processing
 * Returns to normal shell signal handling
 */
void	restore_heredoc_signals(void)
{
	signal(SIGINT, handler);      // Restore main shell handler
	signal(SIGQUIT, sigquit_handler); // Restore main shell handler
}
