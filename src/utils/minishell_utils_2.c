/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 23:54:13 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/15 23:55:06 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

void	clean_mini(t_token *token_list, t_cmd *cmdarg_list)
{
	t_cmd	*tmp;

	tmp = cmdarg_list;
	while (tmp)
	{
		if (tmp->redirections)
			pipe_closed(tmp->redirections);
		tmp = tmp->next;
	}
	free_token_list(token_list);
	free_cmd(cmdarg_list);
}

int	syntax_check(t_token *token_list, t_shell *shell)
{
	t_token	*current;

	current = token_list;
	if (toksizer(token_list) == 1 && rediction_rec(current))
		return (printf(SYNTAX_ERROR), shell->exit_status = 258, -1);
	while (current)
	{
		if (pipe_errors(current))
			return (printf(PIPE_ERROR), shell->exit_status = 258, -1);
		if (redirection_errors(current))
			return (printf(SYNTAX_ERROR), shell->exit_status = 258, -1);
		current = current->next;
	}
	return (0);
}

bool	redirection_errors(t_token *current)
{
	return (rediction_rec(current) && (current->next == NULL
			|| rediction_rec(current->next)));
}

void	pipe_closed(t_redi *redic)
{
	while (redic)
	{
		if (redic->heredoc_fd != -1)
		{
			close(redic->heredoc_fd);
			redic->heredoc_fd = -1;
		}
		redic = redic->next;
	}
}

bool	pipe_errors(t_token *current)
{
	return (current->type == PIPE && ((current->next == NULL
				|| current->next->type == PIPE || current->prev == NULL
				|| rediction_rec(current->prev))));
}
