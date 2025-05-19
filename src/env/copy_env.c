/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:50:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/05/19 18:58:36 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Parse a single environment variable into key and value */
static void    parse_env_var(char *env_str, char **key, char **value)
{
    int     i;

    i = 0;
    while (env_str[i] && env_str[i] != '=')
        i++;

    *key = ft_substr(env_str, 0, i);

    if (env_str[i] == '=')
        *value = ft_strdup(env_str + i + 1);
    else
        *value = ft_strdup("");
}

/* Create a linked list from the environment array */
t_env    *create_env_list(char **env)
{
    t_env   *env_list;
    t_env   *new_node;
    int     i;
    char    *key;
    char    *value;

    env_list = NULL;
    i = 0;

    while (env[i])
    {
        parse_env_var(env[i], &key, &value);
        new_node = new_env_node(key, value);

        if (!new_node)
        {
            // Handle error - cleanup and return
            // You might need to implement a function to free the env_list
            return (NULL);
        }

        add_env_node(&env_list, new_node);
        free(key);
        free(value);
        i++;
    }

    return (env_list);
}

char    *get_username(t_env *env)
{
    char    *username;

    username = get_env_value(env, "USER");
    if (!username)
        username = ft_strdup("user");
    else
        username = ft_strdup(username);
    return (username);
}

char    **split_paths(t_env *env)
{
    char    *path_str;
    char    **paths;

    path_str = get_env_value(env, "PATH");
    if (!path_str)
        return (NULL);
    paths = ft_split(path_str, ':');
    return (paths);
}


