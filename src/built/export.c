/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:15:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/30 13:32:47 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"

static int	is_valid_varname(char *name)
{
	int	i;

	if (!name || !*name)
		return (0);

	// First character must be letter or underscore
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);

	// Rest can be alphanumeric or underscore
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}

	return (1);
}

static void	print_sorted_env(t_env *env)
{
	t_env	*current;

	// Simple implementation without sorting for now
	// TODO: implement proper sorting later if needed
	current = env;
	while (current)
	{
		// Print in declare -x format
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

static int	parse_assignment(char *arg, char **name, char **value)
{
	char	*equals;

	equals = ft_strchr(arg, '=');
	if (!equals)
	{
		*name = ft_strdup(arg);
		*value = NULL;
		return (1);
	}

	*name = ft_substr(arg, 0, equals - arg);
	if (!*name)
		return (0);

	*value = ft_strdup(equals + 1);
	if (!*value && *(equals + 1))
	{
		free(*name);
		return (0);
	}

	return (1);
}

int	builtin_export(t_shell *shell, char **args)
{
	int		i;
	char	*name;
	char	*value;
	t_env	*env_var;
	int		has_valid_args;

	// No arguments - print sorted environment
	if (!args[1])
	{
		print_sorted_env(shell->env);
		return (0);
	}

	// Check if we have any non-empty arguments
	has_valid_args = 0;
	i = 1;
	while (args[i])
	{
		if (args[i][0] != '\0')  // Non-empty argument
		{
			has_valid_args = 1;
			break;
		}
		i++;
	}

	// If all arguments are empty strings (from variable expansion), behave like no arguments
	if (!has_valid_args)
	{
		print_sorted_env(shell->env);
		return (0);
	}

	// Process each argument
	i = 1;
	while (args[i])
	{
		// Skip empty arguments (from variable expansion)
		if (args[i][0] == '\0')
		{
			i++;
			continue;
		}

		// Parse NAME=VALUE format
		if (!parse_assignment(args[i], &name, &value))
			return (1);

		// Check if name is valid
		if (!is_valid_varname(name))
		{
			ft_putstr_fd("export: '", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			free(name);
			if (value)
				free(value);
			return (1);
		}

		// Update or add variable
		env_var = find_env_var(shell->env, name);
		if (env_var)
		{
			// Only update if there's a value
			if (value)
			{
				if (env_var->value)
					free(env_var->value);
				env_var->value = value;
			}
			// If no value provided, just mark as exported (don't change existing value)
		}
		else
			add_env_node(&shell->env, new_env_node(name, value));

		free(name);
		i++;
	}

	return (0);
}
