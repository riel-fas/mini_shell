/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:15:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/13 20:43:58 by riel-fas         ###   ########.fr       */
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
	t_env	*smallest;
	t_env	*temp;
	char	**printed;
	int		count;
	int		i;

	// Count environment variables
	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}

	// Allocate array to track printed variables
	printed = (char **)malloc(sizeof(char *) * (count + 1));
	if (!printed)
		return;

	// Initialize printed array
	i = 0;
	while (i < count)
	{
		printed[i] = NULL;
		i++;
	}

	// Print variables in sorted order
	i = 0;
	while (i < count)
	{
		// Find smallest remaining variable
		smallest = NULL;
		current = env;
		while (current)
		{
			// Skip if already printed
			temp = env;
			while (temp && i > 0)
			{
				if (ft_strcmp(current->key, printed[i - 1]) <= 0)
					break;
				temp = temp->next;
			}

			if (!smallest || ft_strcmp(current->key, smallest->key) < 0)
			{
				// Check if already printed
				int j = 0;
				int is_printed = 0;
				while (j < i)
				{
					if (ft_strcmp(current->key, printed[j]) == 0)
					{
						is_printed = 1;
						break;
					}
					j++;
				}

				if (!is_printed)
					smallest = current;
			}

			current = current->next;
		}

		if (smallest)
		{
			// Mark as printed
			printed[i] = smallest->key;

			// Print in declare -x format
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(smallest->key, 1);
			if (smallest->value)
			{
				ft_putstr_fd("=\"", 1);
				ft_putstr_fd(smallest->value, 1);
				ft_putstr_fd("\"", 1);
			}
			ft_putchar_fd('\n', 1);
		}

		i++;
	}

	free(printed);
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

	// No arguments - print sorted environment
	if (!args[1])
	{
		print_sorted_env(shell->env);
		return (0);
	}

	// Process each argument
	i = 1;
	while (args[i])
	{
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
				free(env_var->value);
				env_var->value = value;
			}
		}
		else
			add_env_node(&shell->env, new_env_node(name, value));

		free(name);
		i++;
	}

	return (0);
}
