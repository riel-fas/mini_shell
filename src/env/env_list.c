/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 18:53:58 by riel-fas          #+#    #+#             */
/*   Updated: 2025/05/19 19:24:34 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

/* Create a new environment variable node */
t_env   *new_env_node(char *name, char *value)
{
    t_env   *new;

    new = (t_env *)malloc(sizeof(t_env));
    if (!new)
        return (NULL);
    new->key = ft_strdup(name);
    new->value = ft_strdup(value);
    new->next = NULL;
    return (new);
}

/* Add a node to the end of the environment list */
void    add_env_node(t_env **env_list, t_env *new_node)
{
    t_env   *temp;

    if (!*env_list)
    {
        *env_list = new_node;
        return;
    }
    temp = *env_list;
    while (temp->next)
        temp = temp->next;
    temp->next = new_node;
}

/* Search for an environment variable by key */
t_env   *find_env_var(t_env *env, char *key)
{
    t_env   *current;

    current = env;
    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
            return (current);
        current = current->next;
    }
    return (NULL);
}

/* Get environment variable value by key */
char    *get_env_value(t_env *env, char *key)
{
    t_env   *var;

    var = find_env_var(env, key);
    if (var)
        return (var->value);
    return (NULL);
}

/* Convert env list to string array (char **) for execve */
char    **env_list_to_array(t_env *env)
{
    t_env   *current;
    char    **env_array;
    int     count;
    int     i;
    char    *temp;

    count = 0;
    current = env;
    while (current && ++count)
        current = current->next;

    env_array = (char **)malloc(sizeof(char *) * (count + 1));
    if (!env_array)
        return (NULL);

    current = env;
    i = 0;
    while (current)
    {
        temp = ft_strjoin(current->key, "=");
        env_array[i] = ft_strjoin(temp, current->value);
        free(temp);
        current = current->next;
        i++;
    }
    env_array[i] = NULL;
    return (env_array);
}
