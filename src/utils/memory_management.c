/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 12:38:00 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 17:59:14 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"
#include "../../includes/memory.h"

/**
 * Free a string array (NULL terminated)
 * @param arr String array to free
 */
void	free_str_array(char **arr)
{
	int	i;

	if (!arr)
		return;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
}

/**
 * Free a token node completely
 * @param token Token to free
 */
void	free_token_node(t_token *token)
{
	if (!token)
		return;
	if (token->value)
		free(token->value);
	if (token->original_value)
		free(token->original_value);
	free(token);
}

/**
 * Free the entire token list
 * @param tokens Head of token list to free
 */
void	free_token_list(t_token *tokens)
{
	t_token	*tmp;
	t_token	*next;

	tmp = tokens;
	while (tmp)
	{
		next = tmp->next;
		free_token_node(tmp);
		tmp = next;
	}
}

/**
 * Free a heredoc list
 * @param heredoc_list Heredoc list to free
 */
void	free_heredoc_list(t_heredoc_list *heredoc_list)
{
	t_heredoc_list	*tmp;
	t_heredoc_list	*next;

	tmp = heredoc_list;
	while (tmp)
	{
		next = tmp->next;
		if (tmp->delimiter)
			free(tmp->delimiter);
		free(tmp);
		tmp = next;
	}
}

/**
 * Free a command node completely
 * @param cmd Command node to free
 */
void	free_command_node(t_cmds *cmd)
{
	if (!cmd)
		return;

	// Free command arguments
	if (cmd->args)
		free_str_array(cmd->args);

	// Free redirection files
	if (cmd->input_file)
		free(cmd->input_file);
	if (cmd->output_file)
		free(cmd->output_file);
	if (cmd->rw_file)
		free(cmd->rw_file);

	// Free heredoc delimiter and list
	if (cmd->heredoc_delimeter)
		free(cmd->heredoc_delimeter);
	if (cmd->heredoc_list)
		free_heredoc_list(cmd->heredoc_list);

	// Close heredoc file descriptor if open
	if (cmd->heredoc_fd > 2)
		close(cmd->heredoc_fd);

	free(cmd);
}

/**
 * Free the entire command list
 * @param commands Head of command list to free
 */
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

/**
 * Free an environment node
 * @param env_node Environment node to free
 */
void	free_env_node(t_env *env_node)
{
	if (!env_node)
		return;

	if (env_node->key)
		free(env_node->key);
	if (env_node->value)
		free(env_node->value);

	free(env_node);
}

/**
 * Free the entire environment list
 * @param env Head of environment list
 */
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

/**
 * Complete cleanup of the shell structure
 * @param shell Shell structure to clean
 */
void	complete_shell_cleanup(t_shell *shell)
{
	if (!shell)
		return;

	// Free environment list
	if (shell->env)
		free_env_list(shell->env);

	// Free username
	if (shell->username)
		free(shell->username);

	// Free path array
	if (shell->path)
		free_str_array(shell->path);

	// Free token list
	if (shell->tokens)
		free_token_list(shell->tokens);

	// Free command list
	if (shell->commands)
		free_command_list(shell->commands);

	// Free shell itself
	free(shell);
}

/**
 * Safe free for a single pointer
 * Sets pointer to NULL after freeing to prevent double-free
 * @param ptr Address of pointer to free
 */
void	safe_free(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}
