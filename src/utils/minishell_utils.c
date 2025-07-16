/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 20:54:44 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 01:39:54 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

void	minishell(char *input, t_list **minienv)
{
	t_token	*token_list;
	t_cmd	*cmdarg_list;
	t_shell	shell;

	set_signal_shell(&shell);
	if (input[0] == '\0')
		return ;
	add_history(input);
	if (!quotes_scan(input, &shell))
		return ;
	token_list = is_string_token(input, *minienv, &shell);
	if (syntax_check(token_list, &shell) == -1)
		return (free_token_list(token_list));
	cmdarg_list = parsing(token_list, *minienv, &shell);
	if (!here_doc_check(cmdarg_list, *minienv, &shell))
		return (clean_mini(token_list, cmdarg_list));
	if (check_builtin(cmdarg_list, minienv, &shell) == 1)
		return (clean_mini(token_list, cmdarg_list));
	if (!execution(cmdarg_list, *minienv, &shell))
		return (clean_mini(token_list, cmdarg_list));
	clean_mini(token_list, cmdarg_list);
}
