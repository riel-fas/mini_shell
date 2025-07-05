/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:19:54 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/05 02:31:13 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/mini_shell.h"

t_global_state	g_state = {0, NULL};

void	sync_shell_state(t_shell *shell)
{
	if (shell)
	{
		g_state.exit_status = shell->exit_status;
		g_state.current_shell = shell;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_shell	*shell;
	int		exit_status;

	(void)argc;
	(void)argv;
	signal(SIGINT, handler);
	signal(SIGQUIT, sigquit_handler);
	shell = shell_init(env);
	if (!shell)
	{
		printf("Error: Failed to initialize shell.\n");
		return (1);
	}
	g_state.current_shell = shell;
	exit_status = minishell_loop(shell);
	cleanup(shell);
	return (exit_status);
}
