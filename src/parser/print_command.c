/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roubelka <roubelka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 00:56:46 by roubelka          #+#    #+#             */
/*   Updated: 2025/06/13 02:01:30 by roubelka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void print_commands(t_cmds *cmds) 
{
    int i;
    while (cmds) {
        printf("Command:\n");
        for (i = 0; cmds->args && cmds->args[i]; i++) {
            printf(" - Arg[%d]: %s\n", i, cmds->args[i]);
        }
        printf(" - Input File: %s\n", cmds->input_file ? cmds->input_file : "(NULL)");
        printf(" - Output File: %s\n", cmds->output_file ? cmds->output_file : "(NULL)");
        printf(" - Append: %d\n", cmds->append_node);
        if (cmds->heredoc_delimeter)
            printf(" - Heredoc Delimiter: %s\n", cmds->heredoc_delimeter);
        cmds = cmds->next;
    }
}

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
        free(cmds);
        cmds = next;
    }
}