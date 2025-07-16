/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:32:07 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:32:13 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

static int	n_parsing(char *str)
{
	int	i;

	i = 2;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static char	*join_arg(char **cmd, int i)
{
	char	*tmp;
	char	*hold;

	hold = ft_strdup("");
	while (cmd[i])
	{
		tmp = ft_strjoin(hold, cmd[i]);
		free(hold);
		hold = tmp;
		if (cmd[i + 1])
		{
			tmp = ft_strjoin(hold, " ");
			free(hold);
			hold = tmp;
		}
		i++;
	}
	return (hold);
}

static void	ft_print_echo(char *tmp, int n_flag)
{
	if (!n_flag)
		ft_putendl_fd(tmp, 1);
	else
		ft_putstr_fd(tmp, 1);
}

int	echo_built(char **cmd, t_cmd *shell)
{
	int		i;
	int		n_flag;
	char	*tmp;

	(void)shell;
	i = 1;
	n_flag = 0;
	tmp = NULL;
	while (cmd[i] && ft_strncmp(cmd[i], "-n", 2) == 0)
	{
		if (!n_parsing(cmd[i]))
			break ;
		n_flag = 1;
		i++;
	}
	tmp = join_arg(cmd, i);
	ft_print_echo(tmp, n_flag);
	free(tmp);
	return (0);
}
