/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:19:54 by riel-fas          #+#    #+#             */
/*   Updated: 2025/05/21 13:01:32 by riel-fas         ###   ########.fr       */
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

////////////////////////////////////////test for env///////////////////////////////////////////////

// /**
//  * Test function to print the environment linked list
//  * @param env The environment linked list
//  */
// void	print_env_list(t_env *env)
// {
// 	t_env *current;

// 	current = env;
// 	printf("\n--- Environment Variables ---\n");
// 	while (current)
// 	{
// 		printf("%s=%s\n", current->key, current->value);
// 		current = current->next;
// 	}
// 	printf("----------------------------\n\n");
// }

// int	main(int argc, char **argv, char **env)
// {
// 	t_shell *shell;

// 	(void)argc;
// 	(void)argv;

// 	// Initialize signal handling
// 	signal(SIGINT, handler);
// 	signal(SIGQUIT, SIG_IGN);

// 	// Initialize shell
// 	shell = shell_init(env);
// 	if (!shell)
// 	{
// 		printf("Failed to initialize shell\n");
// 		return (1);
// 	}

// 	// Test: Print the environment linked list
// 	print_env_list(shell->env);

// 	// Test: Print a specific environment variable
// 	printf("USERNAME: %s\n", shell->username);
// 	printf("HOME: %s\n", get_env_value(shell->env, "HOME"));

// 	// Test: Print the PATH directories
// 	printf("\n--- PATH Directories ---\n");
// 	if (shell->path)
// 	{
// 		for (int i = 0; shell->path[i]; i++)
// 			printf("%d: %s\n", i, shell->path[i]);
// 	}
// 	else
// 		printf("PATH not found or empty\n");

// 	// Cleanup before exiting
// 	cleanup(shell);
// 	return (0);
// }





/////////////////////////



int	main(int argc, char **argv, char **env)
{
	t_shell	*shell;
	int		exit_status;

	(void)argc;
	(void)argv;

	// Set up signal handling
	signal(SIGINT, handler);  // CTRL+C
	signal(SIGQUIT, SIG_IGN); // CTRL+\

	// Initialize the shell
	shell = shell_init(env);
	if (!shell)
	{
		printf("Error: Failed to initialize shell.\n");
		return (1);
	}

	// Run the main shell loop
	exit_status = minishell_loop(shell);

	// Clean up and exit
	cleanup(shell);
	return (exit_status);
}
