/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:32:30 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:32:35 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

int	env_built(t_list **env, t_shell *shell)
{
	t_list	*tmp;

	tmp = *env;
	if (!*env || !env)
	{
		shell->exit_status = 1;
		return (1);
	}
	while (tmp)
	{
		if (tmp->key && ft_strcmp(tmp->key, "_") == 0 && tmp->value)
		{
			free(tmp->value);
			tmp->value = ft_strdup("/usr/bin/env");
		}
		if (tmp->key && tmp->value)
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (1);
}
