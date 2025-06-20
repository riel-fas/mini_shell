/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell_loop.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 10:45:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/20 18:36:57 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"
#include "../includes/lexer.h"

static char	*generate_prompt(t_shell *shell)
{
	(void)shell;
	return (ft_strdup("minishell~> "));
}

static int	process_input(t_shell *shell, char *input)
{
	int	exit_status;

	if (!input || !*input || is_only_whitespace(input))
		return (1);
	if (*input)
		add_history(input);
	if (!process_exit_check(input))
		return (0);
	shell->tokens = tokenize(input);
	if (handle_tokenize_error(shell, input))
		return (1);
	// Expand variables in tokens
	expand_tokens(shell->tokens, shell->env, shell->exit_status);
	if (!check_redirection_syntax(shell->tokens) || !check_pipe_syntax(shell->tokens))
	{
		shell->exit_status = 2;
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		return (1);
	}
	shell->commands = parse_tokens(shell->tokens);
	if (!shell->commands)
	{
		printf("Parse error\n");
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		return (1);
	}
	// print_commands(shell->commands);
	exit_status = execute_commands(shell, shell->commands);
	shell->exit_status = exit_status;
	free_tokens(shell->tokens);
	shell->tokens = NULL;
	free_commands(shell->commands);
	shell->commands = NULL;
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
