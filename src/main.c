/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:19:54 by riel-fas          #+#    #+#             */
/*   Updated: 2025/05/13 15:50:29 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <string.h>

int	main(int ac, char **av, char **env)
{
	    (void)ac;
    	(void)av;
	    (void)env;

	while (1)
	{
		char *line;

		line = readline("minishell$~> "); //prompt
		if (!line)
		{
			//the program keeps running until a Ctrl+C or Ctrl+D
			printf("EXIT\n");
			break ;
		}
		if (strlen(line) > 0) //only if there is something in the cmdline
			add_history(line);	//return back to used cmds


			// init_shell();
			// initialize env list, signals, terminal settings
			// enter the main read–parse–execute loop
			// shell_loop();
			// cleanup and exit
	}
}



// int main(int argc, char **argv, char **envp)
// {
    // (void)argc;
    // (void)argv;
    // initialize env list, signals, terminal settings
    // init_shell();
    // enter the main read–parse–execute loop
    // shell_loop();
    // cleanup and exit
    // return 0;
// }
//
//
// void	shell_loop(void)
// {
	// char *line;
//
	// while (1)
	// {
		// line = readline("minishell$~>");
		// if (!line)
			// break ;
		//
//
//
	// }
//
//
//
//
// }

