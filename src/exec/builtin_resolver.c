/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_resolver.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 14:30:00 by riad              #+#    #+#             */
/*   Updated: 2025/07/03 14:30:00 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

int	is_builtin(char *cmd_name)
{
	if (!cmd_name)
		return (0);
	if (ft_strcmp(cmd_name, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd_name, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd_name, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd_name, "export") == 0)
		return (1);
	if (ft_strcmp(cmd_name, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd_name, "env") == 0)
		return (1);
	if (ft_strcmp(cmd_name, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd_name, ":") == 0)
		return (1);
	return (0);
}

t_builtin_func	get_builtin(char *cmd_name)
{
	if (!cmd_name)
		return (NULL);
	if (ft_strcmp(cmd_name, "cd") == 0)
		return (builtin_cd);
	if (ft_strcmp(cmd_name, "echo") == 0)
		return (builtin_echo);
	if (ft_strcmp(cmd_name, "pwd") == 0)
		return (builtin_pwd);
	if (ft_strcmp(cmd_name, "export") == 0)
		return (builtin_export);
	if (ft_strcmp(cmd_name, "unset") == 0)
		return (builtin_unset);
	if (ft_strcmp(cmd_name, "env") == 0)
		return (builtin_env);
	if (ft_strcmp(cmd_name, "exit") == 0)
		return (builtin_exit);
	if (ft_strcmp(cmd_name, ":") == 0)
		return (builtin_colon);
	return (NULL);
}
