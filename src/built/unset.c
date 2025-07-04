/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:15:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/03 23:43:55 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"

static void	process_unset_arg(t_shell *shell, char *arg, int *status,
		int *path_modified)
{
	if (!arg || !*arg)
		return ;
	if (!is_valid_varname(arg))
		handle_invalid_varname(arg, status);
	else
	{
		if (ft_strcmp(arg, "PATH") == 0)
			*path_modified = 1;
		remove_env_var(&shell->env, arg);
	}
}

int	builtin_unset(t_shell *shell, char **args)
{
	int	i;
	int	status;
	int	path_modified;

	if (!shell || !args || !args[1])
		return (0);
	status = 0;
	path_modified = 0;
	i = 1;
	while (args[i])
	{
		if (!args[i] || !*args[i])
		{
			i++;
			continue ;
		}
		process_unset_arg(shell, args[i], &status, &path_modified);
		i++;
	}
	if (path_modified)
		update_shell_path(shell);
	return (status);
}
