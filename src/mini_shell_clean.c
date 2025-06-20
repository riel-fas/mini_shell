/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell_clean.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 18:00:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/13 20:51:04 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/mini_shell.h"

/* Free the environment linked list */
static void	free_env_list(t_env *env)
{
	t_env	*temp;

	while (env)
	{
		temp = env;
		env = env->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
}

/* Free the path array */
static void	free_path(char **path)
{
	int	i;

	if (!path)
		return ;
	i = 0;
	while (path[i])
	{
		free(path[i]);
		i++;
	}
	free(path);
}

void	cleanup(t_shell *shell)
{
	if (!shell)
		return ;
	free_env_list(shell->env);
	free(shell->username);
	free_path(shell->path);
	if (shell->tokens)
		free_tokens(shell->tokens);
	if (shell->commands)
		free_commands(shell->commands);
	free(shell);
}
