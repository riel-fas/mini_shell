/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:15:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/13 20:44:10 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"

static void	remove_first_node(t_env **env_list, t_env *current)
{
	*env_list = current->next;
	free(current->key);
	free(current->value);
	free(current);
}

static void	remove_env_var(t_env **env_list, char *key)
{
	t_env	*current;
	t_env	*prev;

	if (!env_list || !*env_list || !key)
		return ;
	current = *env_list;
	prev = NULL;
	if (ft_strcmp(current->key, key) == 0)
	{
		remove_first_node(env_list, current);
		return ;
	}
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			prev->next = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

static int	is_valid_varname(char *name)
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

int	builtin_unset(t_shell *shell, char **args)
{
	int	i;
	int	status;

	if (!args[1])
		return (0);
	status = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_varname(args[i]))
		{
			ft_putstr_fd("unset: '", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			status = 1;
		}
		else
			remove_env_var(&shell->env, args[i]);
		i++;
	}
	return (status);
}
