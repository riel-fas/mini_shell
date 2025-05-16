/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_mini_shell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 03:56:58 by riel-fas          #+#    #+#             */
/*   Updated: 2025/05/16 04:06:39 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// free allocated memory in shell
void	cleanup(t_shell *shell)
{
	int	x;

	if (shell->env)
	{
		x= 0;
		while (shell->env)
		{
			free(shell->env[x]);
			x++;
		}
		free(shell->env);
	}
	
}
