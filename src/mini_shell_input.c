/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 00:00:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/13 21:11:19 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/mini_shell.h"

int	is_only_whitespace(char *input)
{
	if (!input)
		return (1);
	while (*input)
	{
		if (*input != ' ' && *input != '\t' && *input != '\n' && *input != '\r')
			return (0);
		input++;
	}
	return (1);
}

int	process_exit_check(char *input)
{
	if (ft_strcmp(input, "exit") == 0)
		return (0);
	return (1);
}

int	handle_tokenize_error(t_shell *shell, char *input)
{
	if (!shell->tokens && *input)
	{
		printf("Error: Failed to tokenize input\n");
		return (1);
	}
	return (0);
}

int	handle_parse_error(t_shell *shell, t_parse_result result)
{
	if (result == PARSE_ERROR_SYNTAX)
	{
		printf("Syntax error\n");
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		return (1);
	}
	else if (result == PARSE_ERROR_MEMORY)
	{
		printf("Memory allocation error\n");
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		return (1);
	}
	return (0);
}
