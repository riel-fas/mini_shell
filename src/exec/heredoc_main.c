/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 02:50:26 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/04 02:50:26 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

int	process_heredocs_after_parsing(t_cmds *commands)
{
	t_cmds	*current;

	current = commands;
	while (current)
	{
		if (process_command_heredocs(current) != 0)
			return (1);
		current = current->next;
	}
	return (0);
}
