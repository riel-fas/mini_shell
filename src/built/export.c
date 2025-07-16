/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:33:58 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:34:05 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

t_list	*check_dup_env(char *key, t_list *env)
{
	t_list	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	key_value(char *cmd, char **key, char **value, t_list **env)
{
	t_list	*dup_key;
	int		is_append;

	dup_key = NULL;
	is_append = 0;
	if (ft_strchr(cmd, '='))
	{
		is_append = handler_exp(cmd, key, value);
		if (is_append == -1)
			return ;
	}
	else
		helper(key, value, cmd);
	dup_key = check_dup_env(*key, *env);
	if (dup_key)
	{
		if (is_append && dup_key->value)
			handle_append(dup_key, key, value);
		else
			alloc_2(dup_key, key, value, cmd);
	}
	else
		ft_lstadd_back(env, ft_lstnew(*key, *value));
}

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=' && str[i] != '+')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	if (str[i] == '+')
	{
		if (!str[i + 1] || str[i + 1] != '=')
			return (0);
	}
	return (1);
}

int	check_error(char *cmd)
{
	char	*equals_pos;
	char	*name_end;
	char	*tmp;

	if (!cmd || !*cmd)
		return (write(2, "export: ` : not a valid identifier\n", 36), 1);
	if (*cmd == '=')
		return (print_invalid_identifier("="));
	equals_pos = ft_strchr(cmd, '=');
	if (!equals_pos)
		name_end = cmd + ft_strlen(cmd);
	else
		name_end = equals_pos;
	if (ft_strchr(cmd, '+'))
		name_end = ft_strchr(cmd, '+');
	tmp = ft_substr(cmd, 0, name_end - cmd);
	if (!tmp)
		return (1);
	if (!is_valid_identifier(tmp))
	{
		free(tmp);
		return (print_invalid_identifier(cmd));
	}
	free(tmp);
	return (0);
}

int	export_built(char **cmd, t_list **env)
{
	char	*key;
	char	*value;
	int		ret;

	ret = 0;
	key = NULL;
	value = NULL;
	if (!env || !*env)
		return (1);
	if (!cmd[1])
		sort_env(env);
	cmd++;
	while (*cmd)
	{
		if (check_error(*cmd) != 0)
			ret = 1;
		else
			key_value(*cmd, &key, &value, env);
		cmd++;
	}
	return (ret);
}
