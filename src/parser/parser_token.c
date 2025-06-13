/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roubelka <roubelka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 20:04:54 by roubelka          #+#    #+#             */
/*   Updated: 2025/06/13 02:08:04 by roubelka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// **head bach ikhalih itabdita ila kan khawi
// **tail bach ikhalih itabdita kol mara tzad chiwahd jdid
t_cmds  *init_new_command(t_cmds **head, t_cmds **tail)
{
    t_cmds  *cmd;
    cmd = malloc(sizeof(t_cmds));// mallo nkol commandhasab sie dyala
    if (!cmd)
        return (NULL);
    cmd->args = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append_node = 0;
	cmd->heredoc_delimeter = NULL;
	cmd->next = NULL;
    
	if (!*head) // if list khawya gadi iwli cmd howa head 
		*head = cmd;
	else 
		(*tail)->next = cmd; //apdait tail bach iwli pointer njdid
	*tail = cmd;
    return (cmd);
}

void	add_arg_to_cmd(t_cmds *cmd, char *value)
{
	int count;
		count = 0; //3adad args likaynin fargs
	while (cmd->args && cmd->args[count])
		count++;
	char **new_args = realloc(cmd->args, sizeof(char *) * (count + 2)); // nzido size dyal args jdida whay(+ 2) 1 dyal args jdid 2 dyal NULL
	if (!new_args)
		return ;
	cmd->args = new_args; // kanhadto cmd->args bjdid lizdna fnew->args
	cmd->args[count] = strdup(value);
	cmd->args[count + 1] = NULL;
}
void handle_redirections(t_cmds *cmd, t_token **tokens) {
    t_token *current = *tokens;

    if (!current || !current->next)
        return;

    if (current->type == TOKEN_REDIR_IN)
        cmd->input_file = ft_strdup(current->next->value);
    else if (current->type == TOKEN_REDIR_OUT) {
        cmd->output_file = ft_strdup(current->next->value);
        cmd->append_node = 0;
    }
    else if (current->type == TOKEN_REDIR_APPEND) {
        cmd->output_file = ft_strdup(current->next->value);
        cmd->append_node = 1;
    }
    else if (current->type == TOKEN_HEREDOC) {
        cmd->heredoc_delimeter = ft_strdup(current->next->value);
    }

    *tokens = current->next; // skip to the next after the filename or delimiter
}

// void handle_redirections(t_cmds *cmd, t_token **tokens) 
// {
//     t_token *tok = *tokens;
//     if (!tok || !tok->next)
//         return;
//     tok = tok->next;
//     if (!tok)
//         return;
//     if ((*tokens)->type == TOKEN_REDIR_IN) 
//     {
//         cmd->input_file = strdup(tok->value);
//     } 
//     else if ((*tokens)->type == TOKEN_REDIR_OUT) 
//     {
//         cmd->output_file = strdup(tok->value);
//         cmd->append_node = 0;
//     } 
//     else if ((*tokens)->type == TOKEN_REDIR_APPEND) 
//     {
//         cmd->output_file = strdup(tok->value);
//         cmd->append_node = 1;
//     }
//     *tokens = tok; // skip the filename token
// }

t_cmds	*parse_tokens(t_token *tokens)
{
	t_cmds	*head = NULL;
	t_cmds	*tail = NULL;
	t_cmds	*current_cmd = NULL;

	while (tokens)
	{
		if (tokens->type == TOKEN_WORD || tokens->type == TOKEN_ARGUMENT)
		{
			if (!current_cmd)
				current_cmd = init_new_command(&head, &tail);
			add_arg_to_cmd(current_cmd, tokens->value);
		}
		else if (tokens->type == TOKEN_PIPE)
		{
			current_cmd = NULL;
		}
		else if (tokens->type == TOKEN_REDIR_IN ||
				 tokens->type == TOKEN_REDIR_OUT ||
				 tokens->type == TOKEN_REDIR_APPEND)
		{
			if (!current_cmd)
				current_cmd = init_new_command(&head, &tail);
			handle_redirections(current_cmd, &tokens);
		}
		else if (tokens->type == TOKEN_PIPE)
		{
			current_cmd = NULL;
		}
		tokens = tokens->next;
	}
	return (head);
}
