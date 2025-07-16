/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:45:18 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:45:26 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

void	free_list(t_list **list)
{
	t_list	*current;
	t_list	*next;

	if (!list || !*list)
		return ;
	current = *list;
	while (current)
	{
		next = current->next;
		if (current->key)
			free(current->key);
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
	*list = NULL;
}

char	*find_path(t_list *path)
{
	t_list	*tmp;

	tmp = path;
	while (tmp)
	{
		if (!ft_strncmp(tmp->key, "PATH", 4))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

void	get_the_path(char **full_path, char **path_cmd, char *p)
{
	char	*tmp;

	tmp = NULL;
	*full_path = NULL;
	while (*path_cmd)
	{
		*full_path = ft_strjoin(*path_cmd, "/");
		if (!*full_path)
			return ;
		tmp = *full_path;
		*full_path = ft_strjoin(*full_path, p);
		free(tmp);
		tmp = NULL;
		if (!*full_path)
			return ;
		if (access(*full_path, X_OK) == 0)
			return ;
		free(*full_path);
		*full_path = NULL;
		path_cmd++;
	}
}

char	*joining_path(char *p)
{
	char	*path;
	char	*tmp;

	tmp = NULL;
	path = getcwd(NULL, 0);
	if (!path)
		return (NULL);
	tmp = ft_strjoin(path, "/");
	if (!tmp)
	{
		free(path);
		return (NULL);
	}
	free(path);
	tmp = my_strjoin(tmp, p);
	if (!tmp)
		return (NULL);
	if (access(tmp, X_OK) == 0)
		return (tmp);
	free(tmp);
	return (NULL);
}

char	*check_exec(char *p, t_list *env, int *no_file)
{
	char	*full_path;
	char	*path;
	char	**path_cmd;
	char	*dir_path;

	full_path = check_full_path(p);
	if (full_path)
		return (full_path);
	path = find_path(env);
	if (path)
	{
		path_cmd = parsing_split(path, ':');
		if (!path_cmd)
			return (NULL);
		get_the_path(&full_path, path_cmd, p);
		free_buitl(path_cmd);
		if (full_path)
			return (full_path);
	}
	else
		*no_file = 1;
	dir_path = joining_path(p);
	if (dir_path)
		return (dir_path);
	return (NULL);
}
