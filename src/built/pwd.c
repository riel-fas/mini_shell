/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:15:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/03 23:26:04 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"

static int	print_usage_error(char *invalid_option)
{
	ft_putstr_fd("pwd: ", 2);
	ft_putstr_fd(invalid_option, 2);
	ft_putendl_fd(": invalid option", 2);
	return (2);
}

static int	parse_pwd_options(char **args)
{
	int	i;
	int	j;

	i = 1;
	while (args[i])
	{
		if (args[i][0] == '-' && args[i][1])
		{
			if (args[i][1] == '-' && !args[i][2])
				break ;
			j = 1;
			while (args[i][j])
			{
				if (args[i][j] != 'L' && args[i][j] != 'P')
					return (print_usage_error(args[i]));
				j++;
			}
		}
		else
			break ;
		i++;
	}
	return (0);
}

int	builtin_pwd(t_shell *shell, char **args)
{
	char	*pwd;
	int		option_result;

	(void)shell;
	option_result = parse_pwd_options(args);
	if (option_result != 0)
		return (option_result);
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
