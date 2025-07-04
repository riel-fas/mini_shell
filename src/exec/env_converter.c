/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_converter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 00:36:10 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 03:45:25 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

static int	count_env_vars(t_env *env)
{
	t_env	*current;
	int		count;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static char	*create_env_string(t_env *env_node)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(env_node->key, "=");
	if (!temp)
		return (NULL);
	if (env_node->value)
		result = ft_strjoin(temp, env_node->value);
	else
		result = ft_strjoin(temp, "");
	free(temp);
	return (result);
}

static int	fill_env_array(char **env_array, t_env *env)
{
	t_env	*current;
	int		i;

	current = env;
	i = 0;
	while (current)
	{
		env_array[i] = create_env_string(current);
		if (!env_array[i])
		{
			free_env_array(env_array);
			return (0);
		}
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (1);
}

char	**convert_env_to_array(t_env *env)
{
	char	**env_array;
	int		count;

	count = count_env_vars(env);
	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	if (!fill_env_array(env_array, env))
		return (NULL);
	return (env_array);
}
