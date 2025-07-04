/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 18:53:58 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/04 00:26:15 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

t_env	*new_env_node(char *name, char *value)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(name);
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	new->next = NULL;
	return (new);
}

void	add_env_node(t_env **env_list, t_env *new_node)
{
	t_env	*temp;

	if (!*env_list)
	{
		*env_list = new_node;
		return ;
	}
	temp = *env_list;
	while (temp->next)
		temp = temp->next;
	temp->next = new_node;
}

t_env	*find_env_var(t_env *env, char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

char	*get_env_value(t_env *env, char *key)
{
	t_env	*var;

	var = find_env_var(env, key);
	if (var)
		return (var->value);
	return (NULL);
}
