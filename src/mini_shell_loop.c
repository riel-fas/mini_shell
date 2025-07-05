/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell_loop.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 17:54:33 by marvin            #+#    #+#             */
/*   Updated: 2025/07/05 02:23:11 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"
#include "../includes/lexer.h"

static int	validate_syntax(t_shell *shell)
{
	if (!check_first_token_redirection(shell))
		return (0);
	if (!check_general_syntax(shell))
		return (0);
	return (1);
}

static void	cleanup_tokens_and_commands(t_shell *shell)
{
	free_tokens(shell->tokens);
	shell->tokens = NULL;
	if (shell->commands)
	{
		free_commands(shell->commands);
		shell->commands = NULL;
	}
}

static int	parse_and_execute(t_shell *shell)
{
	int	exit_status;

	shell->commands = parse_tokens(shell->tokens);
	if (!shell->commands)
	{
		printf("Parse error\n");
		cleanup_tokens_and_commands(shell);
		return (1);
	}
	if (process_heredocs_after_parsing(shell->commands) != 0)
	{
		cleanup_tokens_and_commands(shell);
		return (1);
	}
	exit_status = execute_commands(shell, shell->commands);
	shell->exit_status = exit_status;
	sync_shell_state(shell);
	cleanup_tokens_and_commands(shell);
	return (1);
}

static int	process_input(t_shell *shell, char *input)
{
	if (!input || !*input || is_only_whitespace(input))
		return (1);
	if (*input)
		add_history(input);
	if (!process_exit_check(input))
		return (0);
	shell->tokens = tokenize(input);
	if (handle_tokenize_error(shell, input))
		return (1);
	expand_tokens(shell->tokens, shell->env, shell->exit_status);
	if (all_tokens_empty(shell->tokens))
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		return (1);
	}
	if (!validate_syntax(shell))
		return (1);
	return (parse_and_execute(shell));
}

int	minishell_loop(t_shell *shell)
{
	char	*input;
	char	*prompt;
	int		status;

	status = 1;
	while (status)
	{
		prompt = generate_prompt(shell);
		input = readline(prompt);
		free(prompt);
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		status = process_input(shell, input);
		free(input);
	}
	return (shell->exit_status);
}
