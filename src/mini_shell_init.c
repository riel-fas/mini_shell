/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 04:10:39 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/20 18:23:16 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/mini_shell.h"

t_shell	*shell_init(char **env)
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
