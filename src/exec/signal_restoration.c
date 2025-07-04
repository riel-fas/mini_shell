/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_restoration.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 02:59:01 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/04 02:59:01 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

void	restore_signals(void)
{
	signal(SIGINT, handler);
	signal(SIGQUIT, sigquit_handler);
}
