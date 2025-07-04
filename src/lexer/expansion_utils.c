/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 04:06:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/04 05:12:43 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"

void	toggle_quotes(int c, int *in_single, int *in_double)
{
	if (c == '\'' && !(*in_double))
		*in_single = !(*in_single);
	else if (c == '"' && !(*in_single))
		*in_double = !(*in_double);
}

int	is_var_start(char c)
{
	return (ft_isalpha(c) || c == '_' || c == '?'
		|| c == '$' || ft_isdigit(c));
}

int	copy_var_value(char *var_value, char *result, int j, int result_size)
{
	int	val_len;
	int	space_left;

	val_len = ft_strlen(var_value);
	if (j + val_len + 1 < result_size)
	{
		ft_memcpy(result + j, var_value, val_len);
		return (j + val_len);
	}
	space_left = result_size - j - 1;
	if (space_left > 0)
	{
		ft_memcpy(result + j, var_value, space_left);
		return (j + space_left);
	}
	return (j);
}

/*
 * Gets the value for a given variable name.
 */
char	*get_var_value(char *var_name, t_env *env, int exit_status)
{
	char	*value;

	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(exit_status));
	if (ft_strcmp(var_name, "$") == 0)
		return (ft_strdup("$$"));
	if (ft_strlen(var_name) == 1 && ft_isdigit(var_name[0]))
	{
		if (var_name[0] == '0')
			return (ft_strdup("minishell"));
		else
			return (ft_strdup(""));
	}
	value = get_env_value(env, var_name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}
