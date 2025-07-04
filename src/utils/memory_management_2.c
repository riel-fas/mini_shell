/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:13:20 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 22:15:37 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"
#include "../../includes/memory.h"

void	free_command_list(t_cmds *commands)
{
	t_cmds	*tmp;
	t_cmds	*next;

	tmp = commands;
	while (tmp)
	{
		next = tmp->next;
		free_command_node(tmp);
		tmp = next;
	}
}

void	free_env_node(t_env *env_node)
{
	if (!env_node)
		return ;
	if (env_node->key)
		free(env_node->key);
	if (env_node->value)
		free(env_node->value);
	free(env_node);
}

void	free_env_list(t_env *env)
{
	t_env	*tmp;
	t_env	*next;

	tmp = env;
	while (tmp)
	{
		next = tmp->next;
		free_env_node(tmp);
		tmp = next;
	}
}

void	complete_shell_cleanup(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->env)
		free_env_list(shell->env);
	if (shell->username)
		free(shell->username);
	if (shell->path)
		free_str_array(shell->path);
	if (shell->tokens)
		free_token_list(shell->tokens);
	if (shell->commands)
		free_command_list(shell->commands);
	free(shell);
}

void	safe_free(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}
