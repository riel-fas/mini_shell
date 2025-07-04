/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:15:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/04 00:03:00 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"

static void	print_sorted_env(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(current->key, 1);
		if (current->value)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(current->value, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putchar_fd('\n', 1);
		current = current->next;
	}
}

static int	has_valid_arguments(char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (args[i][0] != '\0')
			return (1);
		i++;
	}
	return (0);
}

int	builtin_export(t_shell *shell, char **args)
{
	int	i;
	int	path_modified;

	if (!args[1] || !has_valid_arguments(args))
	{
		print_sorted_env(shell->env);
		return (0);
	}
	i = 1;
	path_modified = 0;
	while (args[i])
	{
		if (args[i][0] != '\0')
		{
			if (process_export_arg(shell, args[i], &path_modified))
				return (1);
		}
		i++;
	}
	if (path_modified)
		update_shell_path(shell);
	return (0);
}
