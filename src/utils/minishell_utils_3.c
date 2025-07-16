/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils_3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 23:57:59 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/15 23:58:06 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f'
		|| c == '\v');
}

int	is_red_pip(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

bool	check_duplicated(t_lexer *lexer, char op[3])
{
	return ((op[0] == '>' && lexer->pos + 1 < lexer->len
			&& lexer->input[lexer->pos + 1] == '>') || (op[0] == '<'
			&& lexer->pos + 1 < lexer->len && lexer->input[lexer->pos
				+ 1] == '<'));
}

ssize_t	check_dollar(char *str)
{
	ssize_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] != '\0' && (ft_isalnum(str[i + 1])
				|| str[i + 1] == '_' || str[i + 1] == '$' || str[i + 1] == '?'))
			return (i);
		i++;
	}
	return (-1);
}

int	var_size(char *str)
{
	int	i;

	i = 0;
	if (ft_strcmp(str, "?") == 0)
		return (1);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}
