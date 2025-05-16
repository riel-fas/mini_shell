/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roubelka <roubelka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:19:54 by riel-fas          #+#    #+#             */
/*   Updated: 2025/05/16 18:44:10 by roubelka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <string.h>

int	main(int ac, char **av, char **envp)
{
	// t_shell		shell;
	char		*user_input;
	(void)ac;
	(void)av;
	(void)envp;

	while (1)
	{

		user_input = readline("minishell$~> "); //prompt
		if (!user_input)
		{
			//the program keeps running until a  Ctrl+D
			printf("exit\n");
			break ;
		}
		if (strlen(user_input) > 0) //only if there is something in the cmdline
			add_history(user_input);	//return back to used cmds


			//we need to free cause the readline allocate memory so we need to free it so we dont get leaks mem
			free(user_input);
	}
	// cleanup(&shell);
	// return ();// ?EXIT STATUS

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

