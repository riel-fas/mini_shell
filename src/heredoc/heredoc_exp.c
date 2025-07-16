/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_exp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:17:04 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:17:30 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

bool	dollar_char(char *value, ssize_t dollar_pos)
{
	return (value[dollar_pos + 1] == '$' || value[dollar_pos + 1] == '?'
		|| ft_isdigit(value[dollar_pos + 1]));
}

char	*expanding_char(char *value, t_list *minienv, ssize_t dollar_pos)
{
	char	*var;
	char	*expanded;

	var = ft_substr(value, dollar_pos + 1, var_size(value + dollar_pos
				+ 1));
	expanded = var_getting(var, minienv);
	free(var);
	return (expanded);
}

char	*remaining_char_tmp(char *value, ssize_t dollar_pos)
{
	return (ft_substr(value, dollar_pos + var_size(value + dollar_pos + 1)
			+ 1, ft_strlen(value)));
}

void	expanding_exit_char(char **value, t_shell *shell)
{
	ssize_t	dollar_pos;
	char	*expanded;
	char	*temp;
	char	*temp2;
	char	*old_value;

	dollar_pos = check_dollar(*value);
	if ((*value)[dollar_pos + 1] == '?')
		expanded = ft_itoa(shell->exit_status);
	else
		expanded = ft_strdup("");
	temp = str_join_free(ft_substr(*value, 0, dollar_pos), expanded);
	temp2 = ft_substr(*value, dollar_pos + 2, ft_strlen(*value));
	old_value = *value;
	*value = str_join_free(temp, temp2);
	free(old_value);
	free(temp2);
	free(expanded);
}
