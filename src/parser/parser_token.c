/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 20:04:54 by roubelka          #+#    #+#             */
/*   Updated: 2025/06/13 14:33:33 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** 1. zedt TOKEN_HEREDOC f parse_tokens() - heredocs kanو itetrato bhal arguments 3adiyine machia k redirections
** 2. 9adit token advancement f handle_redirections() - kan ghir kayskipi redirection operator, daba kayskipi operator w filename bach maydkhloch
**l filenames k command arguments
** 3. Zedt continue statement mn ba3d handle_redirections() bach maykounch
**    double token advancement li kaydir segfault
*/

#include "../../includes/parser.h"

// **head bach ikhalih itabdita ila kan khawi
// **tail bach ikhalih itabdita kol mara tzad chiwahd jdid
t_cmds  *init_new_command(t_cmds **head, t_cmds **tail)
{
    t_cmds  *cmd;
    cmd = malloc(sizeof(t_cmds));// malloc command sie dyala
    if (!cmd)
        return (NULL);
    cmd->args = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append_node = 0;
	cmd->heredoc_delimeter = NULL;
	cmd->next = NULL;

	if (!*head) // if list khawya khadi iwli cmd howa head
		*head = cmd;
	else
		(*tail)->next = cmd; //adaptit tail bach iwli pointer ljdid
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

    // Men qbel kan ghir kayskipi l current->next, w hada kan kaykhli
    // l filename ytprocessa k command argument. Daba kayskipi both
    // redirection operator W l filename bach maykounch double processing.
    *tokens = current->next->next; // skip both the redirection operator and the filename
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
		//  TOKEN_HEREDOC l redirection handling - men qbel heredoc
		// delimiters kan  itetrato k regular command arguments
		else if (tokens->type == TOKEN_REDIR_IN ||
				 tokens->type == TOKEN_REDIR_OUT ||
				 tokens->type == TOKEN_REDIR_APPEND ||
				 tokens->type == TOKEN_HEREDOC)
		{
			if (!current_cmd)
				current_cmd = init_new_command(&head, &tail);
			handle_redirections(current_cmd, &tokens);
			//  handle_redirections kayadvanci tokens b 2 positions,
			// khassna continue hna bach manijioch tokens = tokens->next f lakher
			continue;
		}
		tokens = tokens->next;
	}
	return (head);
}
