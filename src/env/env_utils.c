/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 00:24:46 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 00:34:59 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

static int	count_env_nodes(t_env *env)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static char	*create_env_string(t_env *node)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(node->key, "=");
	if (!temp)
		return (NULL);
	if (node->value)
		result = ft_strjoin(temp, node->value);
	else
		result = ft_strjoin(temp, "");
	free(temp);
	return (result);
}

static void	cleanup_failed_array(char **env_array, int i)
{
	while (--i >= 0)
		free(env_array[i]);
	free(env_array);
}

static int	fill_env_array(char **env_array, t_env *env, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		env_array[i] = create_env_string(env);
		if (!env_array[i])
			return (0);
		env = env->next;
		i++;
	}
	env_array[i] = NULL;
	return (1);
}

char	**env_list_to_array(t_env *env)
{
	char	**env_array;
	int		count;

	count = count_env_nodes(env);
	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	if (!fill_env_array(env_array, env, count))
	{
		cleanup_failed_array(env_array, count);
		return (NULL);
	}
	return (env_array);
}
