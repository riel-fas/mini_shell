/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_run_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:44:48 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:44:53 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

char	*check_full_path(char *p)
{
	if (!p)
		return (NULL);
	if (*p == '/' || *p == '.')
	{
		if (access(p, X_OK) == 0)
			return (ft_strdup(p));
		else
			return (NULL);
	}
	return (NULL);
}

void	handle_exec_error(char *cmd_path, char *cmd_name)
{
	free(cmd_path);
	free(cmd_name);
	command_error(NULL, "malloc failure\n", 1);
}
