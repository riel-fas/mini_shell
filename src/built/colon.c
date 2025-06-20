/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colon.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 23:40:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/14 00:32:29 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"

/**
 * @brief Built-in colon command - does nothing and returns success
 *
 * The colon (:) is a built-in command that does nothing and always
 * returns an exit status of 0 (success). It's often used as a placeholder
 * or in conditional statements where you need a command that always succeeds.
 *
 * @param shell Pointer to the shell structure (unused)
 * @param args Array of arguments (unused)
 * @return int Always returns 0 (success)
 */
int	builtin_colon(t_shell *shell, char **args)
{
	(void)shell;
	(void)args;
	return (0);
}
