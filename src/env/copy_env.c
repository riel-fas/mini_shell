/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:50:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/05/25 18:59:55 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

/**
 * @brief Kad i9ssem l'environment variable l smiya o l9ima
 *
 * Had fonction katredd string dyal environment variable (bhal "PATH=/bin:/usr/bin")
 * o kat9essmo l key o value. Kat9elleb 3la awel '=' bach t3ref fin ghadi
 * t9essem string. Ila mal9at '=', ghadi tkhelli l value khawya.
 *
 * @param env_str String dyal variable li bghina n9essmo
 * @param key Pointer fash ghadi tkhebbi smiyat l variable (khassek dir free 3liha men be3d)
 * @param value Pointer fash ghadi tkhebbi l9ima dyal variable (khassek dir free 3liha men be3d)
 */
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

/**
 * @brief Kat khle9 linked list men array dyal environment variables
 *
 * Had fonction kat7ewel array dyal environment (envp) l wa7ed linked list
 * dyal t_env structures bash ykon s'hel manipulation dyalo. Katdour 3la kol
 * string fl env, o kat9essmo b parse_env_var() l key o value, o katkhle9
 * nodes jdad b new_env_node(), o tzid hom f linked list b add_env_node().
 *
 * Had fonction kat-allouer memory l kol node, key, o value, o khass dir free
 * 3lihom melli kay salew l'utilisation.
 *
 * @param env Array dyal environment li bghina n7ewloh
 * @return t_env* Ras linked list dyal environment jdid, wla NULL ila kan moshkil
 */
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

/**
 * @brief Kat jbed smiyat l'utilisateur men l'environment
 *
 * Had fonction kat9elleb 3la variable "USER" f environment.
 * Ila l9ato, katrejje3 copy dyalo. Ila mal9ato, katrejje3 "user".
 * Li kayst3mel had fonction khasso idir free 3la string li katrejje3.
 *
 * @param env Linked list dyal environment fash ghadi n9ellbo
 * @return char* String jdida fiha smiyat l'utilisateur
 */
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

/**
 * @brief Kat9essem variable PATH l directory bwahdhom
 *
 * Had fonction katjbed l variable PATH men environment o kat9essmo
 * l array dyal strings, kol string fiha directory men PATH. PATH
 * kay t9essem 3end kol caractère deux points (':'). Ila makansh
 * PATH fl environment, katrejje3 NULL.
 *
 * Li kayst3mel had fonction khasso idir free 3la array li rejj3at
 * o 3la kol string fiha.
 *
 * @param env Linked list dyal environment fash ghadi n9ellbo
 * @return char** Array dyal strings fiha directories dyal PATH,
 *                wla NULL ila mal9inash PATH
 */
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
