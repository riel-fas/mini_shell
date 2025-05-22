/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell_loop.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 10:45:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/05/22 17:08:38 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/mini_shell.h"

static char	*generate_prompt(t_shell *shell)
{
	(void)shell; 
	return (ft_strdup("minishell~> "));
}

static int	process_input(t_shell *shell, char *input)
{
	if (!input || !*input)
		return (1);

	// Add to history if not empty
	if (*input)
		add_history(input);

	// Simple exit command for testing
	if (ft_strcmp(input, "exit") == 0)
		return (0);

	// Tokenize the input
	shell->tokens = tokenize(input);

	// Debug: print tokens
	print_tokens(shell->tokens);

	// TODO: Parse tokens into commands
	// TODO: Execute commands

	// Free tokens when done
	free_tokens(shell->tokens);
	shell->tokens = NULL;

	return (1);
}

//The main shell loop
int	minishell_loop(t_shell *shell)
{
	char	*input;
	char	*prompt;
	int		status;

	status = 1;
	while (status)
	{
		// Generate and display prompt
		prompt = generate_prompt(shell);
		input = readline(prompt);
		free(prompt);

		// Handle EOF (Ctrl+D)
		if (!input)
		{
			printf("exit\n");
			break;
		}

		// Process the input
		status = process_input(shell, input);

		free(input);
	}

	return (shell->exit_status);
}


