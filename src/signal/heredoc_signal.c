/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:00:00 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 00:36:18 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

/**
 * Sets up signal handlers specifically for heredoc input collection.
 * Configures SIGINT to use the heredoc-specific handler and ignores SIGQUIT.
 * Used when child processes are collecting heredoc input to ensure proper
 * termination on interrupt signals.
 */
void	setup_heredoc_signals(void)
{
	signal(SIGINT, handle_heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
}

/**
 * Handles SIGINT signal in child processes during heredoc input.
 * Child processes handling heredoc input should exit immediately
 * when interrupted, so this handler simply exits with status 1.
 */
void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	exit(1);
}

/**
 * Sets up signal handlers for parent process during heredoc processing.
 * Configures signal handling when the parent is coordinating heredoc
 * input collection. Uses specialized handlers to manage interrupts
 * during heredoc operations.
 */
void	setup_parent_heredoc_signals(void)
{
	signal(SIGINT, handle_parent_heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
}

/**
 * Handles SIGINT signal when parent process is managing heredoc input.
 * When Ctrl+C is pressed during heredoc input processing, this handler
 * prints a newline and sets the exit status to indicate interruption.
 */
void	handle_parent_heredoc_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	/* The exit status will be set by the parent process */
}

/**
 * Restores default interactive shell signal handlers.
 * Resets signal handlers back to the main shell configuration
 * after special signal handling contexts (like heredoc) are complete.
 */
void	restore_heredoc_signals(void)
{
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
}
