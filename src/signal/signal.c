/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 05:33:05 by riel-fas          #+#    #+#             */
/*   Updated: 2025/05/21 13:02:23 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


extern int rl_catch_signals; // kat9olo ana variable ja mn library 3la bara

void    handler(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_on_new_line();  // hadi opchn fi readline kat3lmo anaho bdasatr jdid
    rl_replace_line("", 0); // kadir satr jdid okatmsah command lktbti 9bl
    rl_redisplay(); // kat3awd t3rad (prompt) mn jdid
}


// */
// void	handler(int sig)
// {
// 	if (sig == SIGINT)
// 	{
// 		write(1, "\n", 1);         // Print a newline
// 		rl_on_new_line();         // Tell readline we're on a new line
// 		rl_replace_line("", 0);   // Clear the current input line
// 		rl_redisplay();           // Redisplay the prompt
// 	}
// }
