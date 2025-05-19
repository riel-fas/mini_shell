/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:19:54 by riel-fas          #+#    #+#             */
/*   Updated: 2025/05/19 16:35:27 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <string.h>


extern int rl_catch_signals; // kat9olo ana variable ja mn library 3la bara

int	main(int ac, char **av, char **env)
{
	// t_shell		*shell;
	(void)env;  // hanaya kastita b (void) hitach (env) mast3mltachi
	char		*user_input;
	(void)ac;
	(void)av;
	// (void)env;

	// Initialize the shell
	// shell = shell_init(env);
	// if (!shell)
	// {
	// 	printf("Error: Failed to initialize shell\n");
	// 	return (1);
	// }
	signal(SIGQUIT, SIG_IGN); // katgahl ichar ila drti ( Ctrl + \ ) omakypranti walo
    signal(SIGINT, handler);
    rl_catch_signals = 0; // makatkhalich readline ithakm f signal (otkhali tahakom lina)
	while (1)
	{

		user_input = readline("minishell$~>"); //prompt
		if (!user_input)
		{
			//the program keeps running until a Ctrl+D
			printf("exit\n");
			break ;
		}
		if (ft_strlen(user_input) > 0) //only if there is something in the cmdline
			add_history(user_input);	//return back to used cmds


			//we need to free cause the readline allocate memory so we need to free it so we dont get leaks mem
			free(user_input);
	}
	// cleanup(shell);
	// return ();// ?EXIT STATUS
	return (0);

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


// // main.c

// int main(int argc, char **argv, char **envp)
// {
//     t_shell *shell;

//     // ARGC CHECK - Check if there are any arguments
//     // Typically a shell doesn't need arguments to start
//     // but you might want to handle flags like -c for commands


    // // Initialize the shell
    // shell = init_shell(envp);
    // if (!shell)
    // {
    //     printf("Error: Failed to initialize shell\n");
    //     return (1);
    // }

//     // Print initialization info (can be removed later)
//     printf("Shell initialized successfully!\n");
//     printf("Username: %s\n", shell->username);

//     // Here you would start the MINISHELL LOOP
//     // But we'll implement that in the next step

//     // Clean up and exit
//     free_shell(shell);
//     return (0);
// }
