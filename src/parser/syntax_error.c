/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 05:35:11 by roubelka          #+#    #+#             */
/*   Updated: 2025/06/24 21:54:37 by riad             ###   ########.fr       */
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
            tokens->type == TOKEN_REDIR_IN || tokens->type == TOKEN_HEREDOC ||
            tokens->type == TOKEN_REDIR_READ_WRITE)
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
                tokens->next->type == TOKEN_REDIR_READ_WRITE ||
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

    // Check for pipes at the beginning or consecutive pipes
    if (tokens->type == TOKEN_PIPE)
    {
        // If it starts with pipe and next is also pipe, it's ||
        if (tokens->next && tokens->next->type == TOKEN_PIPE)
            printf("Syntax error near unexpected token `||`\n");
        else
            printf("Syntax error near unexpected token `|`\n");
        return 0;
    }

    while (tokens)
    {
        if (tokens->type == TOKEN_PIPE)
        {
            if (!tokens->next || tokens->next->type == TOKEN_PIPE)
            {
                // Check if it's || (two consecutive pipes)
                if (tokens->next && tokens->next->type == TOKEN_PIPE)
                    printf("Syntax error near unexpected token `||`\n");
                else
                    printf("Syntax error near unexpected token `|`\n");
                return 0;
            }
        }
        tokens = tokens->next;
    }
    return 1;
}

/**
 * @brief Checks for standalone logical operators that should be syntax errors
 *
 * This function checks if certain operators like '!' appear alone without
 * being followed by a command, which should be treated as a syntax error
 * similar to bash behavior.
 *
 * @param tokens Linked list of tokens to check
 * @return int 1 if syntax is valid, 0 if there's a syntax error
 */
int check_standalone_operators(t_token *tokens)
{
    t_token *current = tokens;

    while (current)
    {
        // Check if it's a standalone '!' (logical NOT operator)
        if (current->type == TOKEN_WORD && ft_strcmp(current->value, "!") == 0)
        {
            // If '!' is the only token or at the end, it's a syntax error
            if (!current->next || current->next->type == TOKEN_PIPE)
            {
                return (0); // Syntax error - exit silently like bash
            }
        }
        current = current->next;
    }
    return (1);
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
