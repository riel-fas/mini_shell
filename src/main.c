/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:19:54 by riel-fas          #+#    #+#             */
/*   Updated: 2025/05/23 22:15:04 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/mini_shell.h"
#include <string.h>


extern int rl_catch_signals; // kat9olo ana variable ja mn library 3la bara


// void ll()
// {
// 	system("leaks -q minishell");
// }

int	main(int argc, char **argv, char **env)
{
	t_shell	*shell;
	int		exit_status;

	(void)argc;
	(void)argv;

	// atexit(ll);
	// Set up signal handling
	signal(SIGINT, handler);  // CTRL+C
	signal(SIGQUIT, SIG_IGN);
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



