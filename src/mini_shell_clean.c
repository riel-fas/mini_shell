/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell_clean.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 18:00:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/03 17:59:14 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/mini_shell.h"

/**
 * Clean up and free all resources in the shell structure
 * This function now uses the specialized memory management functions
 * @param shell The shell structure to clean up
 */
void	cleanup(t_shell *shell)
{
	if (!shell)
		return ;

	// Use our specialized memory management functions
	complete_shell_cleanup(shell);
}
