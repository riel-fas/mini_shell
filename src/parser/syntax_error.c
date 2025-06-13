/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roubelka <roubelka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 05:35:11 by roubelka          #+#    #+#             */
/*   Updated: 2025/06/02 06:56:13 by roubelka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int check_redirection_syntax(t_token *tokens)
{
    while (tokens)
    {
        if (tokens->type == TOKEN_REDIR_OUT || tokens->type == TOKEN_REDIR_APPEND || tokens->type == TOKEN_REDIR_IN)
        {
            if (!tokens->next)
            {
                printf("Syntax error near unexpected token `newline`\n");
                return 0;
            }

            if (tokens->next->type == TOKEN_REDIR_OUT || 
                tokens->next->type == TOKEN_REDIR_APPEND || 
                tokens->next->type == TOKEN_REDIR_IN)
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
