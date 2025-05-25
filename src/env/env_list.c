/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 18:53:58 by riel-fas          #+#    #+#             */
/*   Updated: 2025/05/25 19:01:30 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/mini_shell.h"

/**
 * @brief Kat khle9 node jdid l environment variable
 *
 * Had fonction katkhedem node jdid dyal t_env structures bash tkhezzen
 * fiha variable dyal environment. Katdir malloc l node o kat-dupliquer
 * name o value dyalo. Ila kan moshkil f malloc, katrejje3 NULL.
 *
 * @param name Smiyat l variable (key)
 * @param value L9ima dyal l variable
 * @return t_env* Node jdida, wla NULL ila kan moshkil f malloc
 */
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

/**
 * @brief Katzid node jdida f l'ekher dyal linked list
 *
 * Had fonction katzid new_node f l'ekher dyal env_list. Ila kan
 * env_list khawi (NULL), kat 3ayet l new_node howa l'awel dyal list.
 * Sinon, katdour 7ta l l'ekher dyal list o katzid fiha new_node.
 *
 * @param env_list Pointer l ras linked list (double pointer bash n9edro
 *                 nbeddlo pointer f cas ila kan khawi)
 * @param new_node Node jdida li bghina nzido f list
 */
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

/**
 * @brief Kat9elleb 3la variable dyal environment b key dyalo
 *
 * Had fonction katdour 3la linked list dyal environment o kat9aren
 * keys m3a key li 3tinaha. Ila l9at match, katrejje3 node kamla.
 * Ila mal9at walo, katrejje3 NULL.
 *
 * @param env Linked list dyal environment fash ghadi n9ellbo
 * @param key Smiyat l variable li kan9ellbo 3liha
 * @return t_env* Node li 3endha nefs key, wla NULL ila mal9atha
 */
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

/**
 * @brief Katjbed l9ima dyal variable dyal environment b key dyalo
 *
 * Had fonction kat3ayet l find_env_var bash tl9a node li fiha key
 * li bghina, o katrejje3 ghir value dyalha. Ila mal9at node, katrejje3 NULL.
 *
 * @param env Linked list dyal environment
 * @param key Smiyat l variable li bghina value dyalha
 * @return char* Value dyal variable, wla NULL ila mal9atha
 */
char    *get_env_value(t_env *env, char *key)
{
    t_env   *var;

    var = find_env_var(env, key);
    if (var)
        return (var->value);
    return (NULL);
}

/**
 * @brief Kat7ewel linked list dyal env l array dyal strings bash nestekhdemha m3a execve
 *
 * Had fonction kat7eseb 9edash kayn men node f linked list, katdir malloc
 * l array dyal strings, o katkhdem strings bhal "KEY=VALUE" l kol variable.
 * Katkhessek dir free 3la array + kol string fih melli tsali ste3malo.
 * F l'ekher dyal array katzid NULL bash ykoun conforme m3a format envp.
 *
 * @param env Linked list dyal environment
 * @return char** Array dyal strings "KEY=VALUE", wla NULL ila kan moshkil f malloc
 */
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
