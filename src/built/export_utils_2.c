/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:33:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:33:20 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

int	print_invalid_identifier(char *cmd)
{
	write(2, "export: `", 9);
	write(2, cmd, ft_strlen(cmd));
	write(2, "': not a valid identifier\n", 26);
	return (1);
}

void	helper(char **key, char **value, char *cmd)
{
	*key = ft_strdup(cmd);
	*value = NULL;
}

void	handle_append(t_list *dup_key, char **key, char **value)
{
	char	*tmp;

	if (!dup_key || !key || !value || !*value)
		return ;
	tmp = dup_key->value;
	if (!tmp)
		dup_key->value = ft_strdup(*value);
	else
	{
		dup_key->value = ft_strjoin(tmp, *value);
		free(tmp);
	}
	free(*key);
	free(*value);
	*key = NULL;
	*value = NULL;
}

int	handler_exp(char *cmd, char **key, char **value)
{
	int		is_append;
	char	*plus_pos;
	char	*equals_pos;

	is_append = 0;
	plus_pos = ft_strchr(cmd, '+');
	equals_pos = ft_strchr(cmd, '=');
	if (plus_pos && *(plus_pos + 1) == '=')
	{
		is_append = 1;
		*key = ft_substr(cmd, 0, plus_pos - cmd);
		*value = ft_substr(cmd, (plus_pos - cmd) + 2, ft_strlen(cmd) - (plus_pos
					- cmd) - 2);
	}
	else if (equals_pos)
		alloc_key_value(cmd, key, value, equals_pos);
	if (!*key)
	{
		if (*value)
			free(*value);
		return (-1);
	}
	return (is_append);
}
