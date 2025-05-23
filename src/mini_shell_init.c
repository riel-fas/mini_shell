/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 04:10:39 by riel-fas          #+#    #+#             */
/*   Updated: 2025/05/23 19:06:40 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/mini_shell.h"

t_shell    *shell_init(char **env)
{
	t_shell	*shell;

	shell = (t_shell *)malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	ft_memset(shell, 0, sizeof(t_shell));
	shell->env = create_env_list(env);
	shell->username = get_username(shell->env);
	shell->path = split_paths(shell->env);
	shell->exit_status = 0;
	return (shell);
}
