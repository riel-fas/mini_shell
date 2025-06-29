/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 00:56:46 by roubelka          #+#    #+#             */
/*   Updated: 2025/06/29 17:46:12 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"

void free_commands(t_cmds *cmds)
{
    while (cmds) {
        t_cmds *next = cmds->next;
        for (int i = 0; cmds->args && cmds->args[i]; i++) {
            free(cmds->args[i]);
        }
        free(cmds->args);
        free(cmds->input_file);
        free(cmds->output_file);
        free(cmds->rw_file);
        free(cmds->heredoc_delimeter);
        free(cmds);
        cmds = next;
    }
}
