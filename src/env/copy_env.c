/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:50:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/04 00:21:18 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

static void	parse_env_var(char *env_str, char **key, char **value)
{
	int	i;

	i = 0;
	while (env_str[i] && env_str[i] != '=')
		i++;
	*key = ft_substr(env_str, 0, i);
	if (env_str[i] == '=')
		*value = ft_strdup(env_str + i + 1);
	else
		*value = ft_strdup("");
}

t_env	*create_env_list(char **env)
{
	t_env	*env_list;
	t_env	*new_node;
	int		i;
	char	*key;
	char	*value;

	env_list = NULL;
	i = 0;
	while (env[i])
	{
		parse_env_var(env[i], &key, &value);
		new_node = new_env_node(key, value);
		if (!new_node)
			return (NULL);
		add_env_node(&env_list, new_node);
		free(key);
		free(value);
		i++;
	}
	return (env_list);
}

char	*get_username(t_env *env)
{
	char	*username;

	username = get_env_value(env, "USER");
	if (!username)
		username = ft_strdup("user");
	else
		username = ft_strdup(username);
	return (username);
}

char	**split_paths(t_env *env)
{
	char	*path_str;
	char	**paths;

	path_str = get_env_value(env, "PATH");
	if (!path_str)
		return (NULL);
	paths = ft_split(path_str, ':');
	return (paths);
}

void	update_shell_path(t_shell *shell)
{
	char	**new_path;
	int		i;

	if (!shell)
		return ;
	if (shell->path)
	{
		i = 0;
		while (shell->path[i])
		{
			free(shell->path[i]);
			i++;
		}
		free(shell->path);
	}
	new_path = split_paths(shell->env);
	shell->path = new_path;
}
