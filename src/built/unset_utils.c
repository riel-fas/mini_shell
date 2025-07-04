/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 23:45:00 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 23:43:55 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"

void	remove_first_node(t_env **env_list, t_env *current)
{
	if (!env_list || !current)
		return ;
	*env_list = current->next;
	if (current->key)
		free(current->key);
	if (current->value)
		free(current->value);
	free(current);
}

void	remove_middle_node(t_env *prev, t_env *current)
{
	if (prev)
		prev->next = current->next;
	free(current->key);
	free(current->value);
	free(current);
}

void	remove_env_var(t_env **env_list, char *key)
{
	t_env	*current;
	t_env	*prev;

	if (!env_list || !*env_list || !key)
		return ;
	current = *env_list;
	prev = NULL;
	if (current && ft_strcmp(current->key, key) == 0)
	{
		remove_first_node(env_list, current);
		return ;
	}
	while (current)
	{
		if (current->key && ft_strcmp(current->key, key) == 0)
		{
			remove_middle_node(prev, current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	is_valid_varname(char *name)
{
	int	i;

	if (!name || !*name)
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	handle_invalid_varname(char *arg, int *status)
{
	ft_putstr_fd("unset: '", 2);
	if (arg)
		ft_putstr_fd(arg, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	*status = 1;
}
