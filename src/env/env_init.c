/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:18:50 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:19:26 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

int	parse_env(char *env_var, char **key, char **value)
{
	char	*equal_sign;
	size_t	key_len;

	equal_sign = ft_strchr(env_var, '=');
	if (!equal_sign)
		return (0);
	key_len = equal_sign - env_var;
	*key = ft_substr(env_var, 0, key_len);
	*value = ft_substr(env_var, key_len + 1, ft_strlen(env_var) - key_len - 1);
	if (!(*key) || !(*value))
	{
		free(*key);
		free(*value);
		return (0);
	}
	return (1);
}

t_list	*init_env(char **env)
{
	t_list	*minienv;
	t_list	*new_node;
	char	*key;
	char	*value;
	int		i;

	minienv = NULL;
	if (!env || !*env)
		return (set_env_built(&minienv), minienv);
	i = 0;
	while (env && env[i])
	{
		if (!parse_env(env[i], &key, &value))
		{
			i++;
			continue ;
		}
		new_node = ft_lstnew(key, value);
		if (!new_node)
			return (free(key), free(value), NULL);
		ft_lstadd_back(&minienv, new_node);
		i++;
	}
	return (minienv);
}
