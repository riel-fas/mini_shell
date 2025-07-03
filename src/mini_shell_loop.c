/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell_loop.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 17:54:33 by marvin            #+#    #+#             */
/*   Updated: 2025/07/03 02:34:48 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"
#include "../includes/lexer.h"

// Configure readline to not interfere with our quote processing
// static void	configure_readline(void)
// {
// 	// Disable readline's quote processing completely
// 	rl_completer_quote_characters = NULL;
// 	// Disable filename completion
// 	rl_attempted_completion_function = NULL;
// 	// Disable automatic quote removal
// 	rl_completion_suppress_quote = 1;
// 	// Disable special handling of quotes
// 	rl_special_prefixes = NULL;
// }

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

	// Check if all tokens became empty after expansion
	if (all_tokens_empty(shell->tokens))
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		return (1);
	}

	if (!check_redirection_syntax(shell->tokens) || !check_pipe_syntax(shell->tokens) || !check_unsupported_operators(shell->tokens))
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

	// Process heredocs immediately after parsing to show prompts
	if (process_heredocs_after_parsing(shell->commands) != 0)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		free_commands(shell->commands);
		shell->commands = NULL;
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

	// configure_readline();
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
		// Debug: print status to see what's happening
		// printf("DEBUG: process_input returned: %d\n", status);
	}
	return (shell->exit_status);
}
