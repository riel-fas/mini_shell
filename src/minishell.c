/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 20:53:07 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 01:17:30 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/mini_shell.h"

int	main(int ac, char **av, char **env)
{
	t_list	*minienv;
	char	*input;

	(void)av;
	(void)ac;
	rl_catch_signals = 0;
	handle_signals();
	minienv = init_env(env);
	while (1)
	{
		input = readline("minishell~> ");
		if (input == NULL)
		{
			printf("exit\n");
			break ;
		}
		minishell(input, &minienv);
		free(input);
	}
	return (0);
}
