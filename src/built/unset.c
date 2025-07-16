/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:38:43 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:38:49 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

int	remove_env_node(t_list **env_list, t_list *node)
{
	if (!node)
		return (1);
	if (node->prev)
		node->prev->next = node->next;
	else
		*env_list = node->next;
	if (node->next)
		node->next->prev = node->prev;
	if (node->key)
		free(node->key);
	if (node->value)
		free(node->value);
	free(node);
	return (1);
}

int	set_env_built(t_list **env)
{
	char	*cwd;
	char	cwd_buffer[1024];

	if (!*env || !env)
	{
		if (getcwd(cwd_buffer, sizeof(cwd_buffer)) != NULL)
			cwd = ft_strdup(cwd_buffer);
		else
			cwd = ft_strdup("/Users/yagame");
		ft_lstadd_back(env, ft_lstnew(ft_strdup("OLDPWD"), NULL));
		ft_lstadd_back(env, ft_lstnew(ft_strdup("PWD"), cwd));
		ft_lstadd_back(env, ft_lstnew(ft_strdup("SHLVL"), ft_strdup("1")));
		ft_lstadd_back(env, ft_lstnew(ft_strdup("_"),
				ft_strdup("/usr/bin/env")));
		return (1);
	}
	return (0);
}

int	unset_built(char **cmd, t_list **env)
{
	t_list	*tmp;
	t_list	**env_ptr;
	t_list	*to_remove;

	if (!cmd || !*cmd || !env || !*env)
		return (1);
	env_ptr = env;
	while (*cmd)
	{
		tmp = *env_ptr;
		while (tmp)
		{
			if (ft_strcmp(*cmd, "_") && ft_strcmp(tmp->key, *cmd) == 0)
			{
				to_remove = tmp;
				tmp = tmp->next;
				remove_env_node(env_ptr, to_remove);
				break ;
			}
			else
				tmp = tmp->next;
		}
		cmd++;
	}
	return (1);
}
