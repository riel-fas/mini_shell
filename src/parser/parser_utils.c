/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 00:56:46 by roubelka          #+#    #+#             */
/*   Updated: 2025/07/03 02:34:48 by codespace        ###   ########.fr       */
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

        // Close heredoc file descriptor if still open
        if (cmds->heredoc_fd >= 0) {
            close(cmds->heredoc_fd);
        }

        // Free heredoc list
        while (cmds->heredoc_list) {
            t_heredoc_list *temp = cmds->heredoc_list;
            cmds->heredoc_list = cmds->heredoc_list->next;
            free(temp->delimiter);
            free(temp);
        }

        free(cmds);
        cmds = next;
    }
}
