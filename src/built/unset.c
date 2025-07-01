/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:15:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/01 10:54:06 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"

static void	remove_first_node(t_env **env_list, t_env *current)
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

static void	remove_env_var(t_env **env_list, char *key)
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
			if (prev)
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
	int	path_modified;

	if (!shell || !args || !args[1])
		return (0);
	status = 0;
	path_modified = 0;
	i = 1;
	while (args[i])
	{
		// Skip empty arguments (bash behavior)
		if (!args[i] || !*args[i])
		{
			i++;
			continue;
		}
		if (!is_valid_varname(args[i]))
		{
			ft_putstr_fd("unset: '", 2);
			if (args[i])
				ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			status = 1;
		}
		else
		{
			// Check if we're unsetting PATH
			if (ft_strcmp(args[i], "PATH") == 0)
				path_modified = 1;
			remove_env_var(&shell->env, args[i]);
		}
		i++;
	}

	// Update shell path if PATH was modified
	if (path_modified)
		update_shell_path(shell);

	return (status);
}
