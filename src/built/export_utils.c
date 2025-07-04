/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 23:46:14 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 00:03:00 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"

int	parse_assignment(char *arg, char **name, char **value)
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

static void	handle_export_error(char *arg, char *name, char *value)
{
	ft_putstr_fd("export: '", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	free(name);
	if (value)
		free(value);
}

static void	update_env_var(t_env *env_var, char *name, char *value,
	int *path_mod)
{
	if (ft_strcmp(name, "PATH") == 0)
		*path_mod = 1;
	if (env_var->value)
		free(env_var->value);
	env_var->value = value;
}

static void	add_new_env_var(t_shell *shell, char *name, char *value,
	int *path_mod)
{
	if (ft_strcmp(name, "PATH") == 0)
		*path_mod = 1;
	add_env_node(&shell->env, new_env_node(name, value));
}

int	process_export_arg(t_shell *shell, char *arg, int *path_modified)
{
	char	*name;
	char	*value;
	t_env	*env_var;

	if (!parse_assignment(arg, &name, &value))
		return (1);
	if (!is_valid_varname(name))
	{
		handle_export_error(arg, name, value);
		return (1);
	}
	env_var = find_env_var(shell->env, name);
	if (env_var && value)
		update_env_var(env_var, name, value, path_modified);
	else if (!env_var)
		add_new_env_var(shell, name, value, path_modified);
	free(name);
	return (0);
}
