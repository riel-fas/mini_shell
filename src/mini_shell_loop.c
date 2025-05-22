/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell_loop.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 10:45:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/05/22 13:08:01 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/mini_shell.h"

/**
 * Generate the shell prompt
 * Creates a simple prompt "minishell~> "
 *
 * @param shell The shell structure (unused)
 * @return Dynamically allocated prompt string (must be freed)
 */
static char	*generate_prompt(t_shell *shell)
{
	(void)shell; // Avoid unused parameter warning
	return (ft_strdup("minishell~> "));
}

/**
 * Process user input
 * This will be expanded as you implement lexer, parser, etc.
 *
 * @param shell The shell structure
 * @param input The user input string
 * @return 1 to continue the shell loop, 0 to exit
 */
static int	process_input(t_shell *shell, char *input)
{
	(void)shell;
	if (!input || !*input)
		return (1);

	// Add to history if not empty
	if (*input)
		add_history(input);

	// Simple exit command for testing
	if (ft_strcmp(input, "exit") == 0)
		return (0);

	// TODO: Lexical analysis (tokenize input)
	// TODO: Parsing (create command structure)
	// TODO: Execute commands

	// For now just echo the command for testing
	printf("Command entered: '%s'\n", input);

	return (1);
}

/**
 * The main shell loop
 * Repeatedly displays prompt, reads input, and processes commands
 *
 * @param shell The shell structure
 * @return The exit status
 */
/* Update the process_input function */

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
