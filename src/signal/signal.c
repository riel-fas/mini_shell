/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roubelka <roubelka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 05:33:05 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/01 00:31:42 by roubelka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"


extern int rl_catch_signals; // kat9olo ana variable ja mn library 3la bara

void    handler(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_on_new_line();  // hadi opchn fi readline kat3lmo anaho bdasatr jdid
    rl_replace_line("", 0); // kadir satr jdid okatmsah command lktbti 9bl
    rl_redisplay(); // kat3awd t3rad (prompt) mn jdid
}

void sigquit_handler(int signum) 
{
    printf("Quit: %d\n", signum);
}