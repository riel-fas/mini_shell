/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_tok.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:15:18 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:15:24 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

void	add_toekn_back(t_token **token_list, t_token *token)
{
	t_token	*tmp;

	if (!token_list || !token)
		return ;
	if (*token_list == NULL)
	{
		*token_list = token;
		return ;
	}
	tmp = *token_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = token;
	token->prev = tmp;
	token->next = NULL;
}

void	expanding_var_char(char **value, t_list *minienv, t_shell *shell)
{
	ssize_t	dollar_pos;
	char	*expanded;
	char	*temp;
	char	*temp2;
	char	*old_value;

	dollar_pos = check_dollar(*value);
	while (dollar_pos != -1)
	{
		if (dollar_char(*value, dollar_pos))
		{
			expanding_exit_char(value, shell);
			dollar_pos = check_dollar(*value);
			continue ;
		}
		expanded = expanding_char(*value, minienv, dollar_pos);
		temp = str_join_free(ft_substr(*value, 0, dollar_pos), expanded);
		temp2 = remaining_char_tmp(*value, dollar_pos);
		old_value = *value;
		*value = str_join_free(temp, temp2);
		free(old_value);
		free(temp2);
		free(expanded);
		dollar_pos = check_dollar(*value);
	}
}

bool	check_if_cmd(t_token *current)
{
	return (current->type == WORD || current->type == DOUBLE_QUOTE
		|| current->type == SINGLE_QUOTE);
}
