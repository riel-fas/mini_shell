/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell_loop.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 10:45:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/05/21 18:33:07 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/mini_shell.h"

/**
 * Generate the shell prompt
 * Creates a custom prompt with username and current directory
 *
 * @param shell The shell structure
 * @return Dynamically allocated prompt string (must be freed)
 */
static char	*generate_prompt(t_shell *shell)
{
	char	*prompt;
	char	*cwd;
	char	*dir_name;
	char	*temp;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (ft_strdup("minishell$ "));

	// Get the last component of the path
	dir_name = ft_strrchr(cwd, '/');
	if (!dir_name || *(dir_name + 1) == '\0')
		dir_name = cwd;
	else
		dir_name++;

	// Format: username@directory$
	temp = ft_strjoin(shell->username, "@");
	if (!temp)
	{
		free(cwd);
		return (ft_strdup("minishell$ "));
	}

	prompt = ft_strjoin(temp, dir_name);
	free(temp);
	free(cwd);

	if (!prompt)
		return (ft_strdup("minishell$ "));

	temp = prompt;
	prompt = ft_strjoin(temp, "$ ");
	free(temp);

	return (prompt);
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
