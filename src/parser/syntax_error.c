/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 05:35:11 by roubelka          #+#    #+#             */
/*   Updated: 2025/06/13 14:35:57 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** 1. Zedt TOKEN_HEREDOC l redirection syntax validation - heredoc syntax
**    errors bhal "cat << |" aw "cat << >>" makanch kayetchk
** 2. TOKEN_PIPE validation mn ba3d redirections - kaymen3 invalid
**    syntax bhal "cat < |" mn validation
*/

#include "../../includes/parser.h"

int check_redirection_syntax(t_token *tokens)
{
    while (tokens)
    {
        //TOKEN_HEREDOC l syntax checking - men qbel heredoc syntax
        // errors makanch kayetchkap (mithal: "cat << |" kan kaydouz validation)
        if (tokens->type == TOKEN_REDIR_OUT || tokens->type == TOKEN_REDIR_APPEND ||
            tokens->type == TOKEN_REDIR_IN || tokens->type == TOKEN_HEREDOC)
        {
            if (!tokens->next)
            {
                printf("Syntax error near unexpected token `newline`\n");
                return 0;
            }

            // TOKEN_HEREDOC w TOKEN_PIPE l invalid following tokens
            // Hada kay errors bhal "cat < >" aw "cat < |" aw "cat << <<"
            if (tokens->next->type == TOKEN_REDIR_OUT ||
                tokens->next->type == TOKEN_REDIR_APPEND ||
                tokens->next->type == TOKEN_REDIR_IN ||
                tokens->next->type == TOKEN_HEREDOC ||
                tokens->next->type == TOKEN_PIPE)
            {
                printf("Syntax error near unexpected token `%s`\n", tokens->next->value);
                return 0;
            }
        }
        tokens = tokens->next;
    }
    return 1;
}

int check_pipe_syntax(t_token *tokens)
{
    if (!tokens)
        return 1;
    if (tokens->type == TOKEN_PIPE)
    {
        printf("Syntax error near unexpected token `|`\n");
        return 0;
    }
    while (tokens)
    {
        if (tokens->type == TOKEN_PIPE)
        {
            if (!tokens->next || tokens->next->type == TOKEN_PIPE)
            {
                printf("Syntax error near unexpected token `|`\n");
                return 0;
            }
        }
        tokens = tokens->next;
    }
    return 1;
}
//   ==== fih mochkil bsbab lexer ====

// int check_quotes_closed(t_token *tokens)
// {
//     while (tokens)
//     {
//         if (tokens->type == TOKEN_UNCLOSED_QUOTE)
//         {
//             printf("Syntax error: unclosed quote\n");
//             return 0;
//         }
//         tokens = tokens->next;
//     }
//     return 1;
// }
