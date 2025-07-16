/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:30:41 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:31:05 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

void	free_buitl(char **cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return ;
	while (cmd[i])
	{
		free(cmd[i]);
		i++;
	}
	free(cmd);
}

int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (0);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (0);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (0);
	else if (ft_strcmp(cmd, "export") == 0)
		return (0);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (0);
	else if (ft_strcmp(cmd, "env") == 0)
		return (0);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (0);
	return (-1);
}

int	run(t_cmd *cmd, t_list **env, t_shell *shell)
{
	int	result;

	result = 0;
	if (ft_strcmp(cmd->cmd[0], "echo") == 0)
		result = echo_built(cmd->cmd, cmd);
	else if (ft_strcmp(cmd->cmd[0],
			"cd") == 0)
		result = cd_built(cmd->cmd, env);
	else if (ft_strcmp(cmd->cmd[0],
			"pwd") == 0)
		result = pwd_built(env);
	else if (ft_strcmp(cmd->cmd[0],
			"export") == 0)
		result = export_built(cmd->cmd, env);
	else if (ft_strcmp(cmd->cmd[0],
			"unset") == 0)
		result = unset_built(cmd->cmd, env);
	else if (ft_strcmp(cmd->cmd[0],
			"env") == 0)
		result = env_built(env, shell);
	else if (ft_strcmp(cmd->cmd[0],
			"exit") == 0)
		result = exit_built(cmd->cmd, env, shell);
	shell->exit_status = result;
	return (1);
}
