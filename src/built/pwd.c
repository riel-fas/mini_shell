/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:15:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/13 20:44:04 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"

int	builtin_pwd(t_shell *shell, char **args)
{
	char	*pwd;

	(void)shell;
	(void)args;
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		ft_putendl_fd("pwd: error retrieving current directory", 2);
		return (1);
	}
	ft_putendl_fd(pwd, 1);
	free(pwd);
	return (0);
}
