/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:19:54 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/20 18:20:58 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/mini_shell.h"

// void ll()
// {
// 	system("leaks -q minishell");
	// atexit(ll);
// }

extern int rl_catch_signals;

int	main(int argc, char **argv, char **env)
{
	t_shell	*shell;
	int		exit_status;

	(void)argc;
	(void)argv;
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
	rl_catch_signals = 0;
	shell = shell_init(env);
	if (!shell)
	{
		printf("Error: Failed to initialize shell.\n");
		return (1);
	}
	exit_status = minishell_loop(shell);
	cleanup(shell);
	return (exit_status);
}
