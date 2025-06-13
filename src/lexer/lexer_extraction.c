/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 19:15:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/13 12:57:55 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"

/**
 * @brief Katjbed token dyal WORD men input string
 *
 * Had fonction katsift lik kelma men input strings. Katbda men position 'i'
 * o katkhdem 7ta tl9a chi whitespace, operator wla quote. L7arf lli m9adiyin
 * kaysejmihom f variable 'word' o katrejje3ha. Katbeddel pointer 'i' bash
 * parser i9der ikhdeem m3a l7arf lli jay.
 *
 * @param input String li ghadi n9ellbo fiha 3la kelma
 * @param i Pointer l position fash ghadin nbdaw n9ellbo, kaytwella
 *          position jdida melli tsali fonction
 * @return char* Kelma li tl9at, khassek dir liha free men be3d
 */
char	*extract_word(char *input, int *i)
{
	int		start;
	int		len;
	char	*word;

	start = *i;
	len = 0;
	while (input[*i] && !is_whitespace(input[*i]) &&
			!is_operator(input[*i]) && !is_quote(input[*i]))
	{
		(*i)++;
		len++;
	}
	word = ft_substr(input, start, len);
	return (word);
}


/**
 * @brief Fonction d'aide bash tredd operator o t3ayyet l type dyalo
 *
 * Had fonction d'aide ka tset3emal m3a extract_operator bash tersalissi
 * l'opérateur. Kat7added type dyal token, katzid l'indice 'i' b increment
 * li 3tinaha, o katrejje3 copie men string l'opérateur.
 *
 * @param op String dyal operator (">>", "<", etc.)
 * @param token_type Type dyal token li ghadi nrejje3o (TOKEN_REDIR_OUT, etc.)
 * @param i Pointer l position f string input, kanbedloh b increment
 * @param increment Ch7al ghadin nzido l position (1 wla 2 7sb size dyal operator)
 * @param type Pointer l variable li ghadi n7ettu fiha type dyal token
 * @return char* Copie men string dyal operator
 */
static char	*set_operator(char *op, t_token_type token_type, int *i, int increment, t_token_type *type)
{
	*i += increment;
	*type = token_type;
	return (ft_strdup(op));
}

/**
 * @brief Katjbed operator token men input string
 *
 * Had fonction kat3ref chi operator (>, >>, <, <<, |) f position 'i'
 * f input. Katrejje3 string dyal operator o kat7ett type dyalo f variable
 * 'type'. Katzid 'i' b 1 wla 2 7sb size dyal operator. Ila mal9at
 * operator, katrejje3 NULL o kat7ett TOKEN_WORD f type.
 *
 * @param input String li ghadi n9ellbo fiha 3la operator
 * @param i Pointer l position fash ghadin n9ellbo, kaytwella position jdida
 * @param type Pointer l variable li ghadi n7ettu fiha type dyal token
 * @return char* String dyal operator li tl9a, wla NULL ila mal9a 7tta operator
 */
char	*extract_operator(char *input, int *i, t_token_type *type)
{
	if (input[*i] == '>' && input[*i + 1] == '>')
		return (set_operator(">>", TOKEN_REDIR_APPEND, i, 2, type));
	else if (input[*i] == '<' && input[*i + 1] == '<')
		return (set_operator("<<", TOKEN_HEREDOC, i, 2, type));
	else if (input[*i] == '>')
		return (set_operator(">", TOKEN_REDIR_OUT, i, 1, type));
	else if (input[*i] == '<')
		return (set_operator("<", TOKEN_REDIR_IN, i, 1, type));
	else if (input[*i] == '|')
		return (set_operator("|", TOKEN_PIPE, i, 1, type));
	*type = TOKEN_WORD;
	return (NULL);
}


/**
 * @brief Katjbed string li ma bin quotes men input
 *
 * Had fonction katakhod l'input o katbda men position 'i' (lli normalement
 * kaykoun ghir b3d quote), o kat9ra 7ta lakhir dial quote. Katdir check ila
 * quote tseddatt mezyana, o katrejje3 l'contenu li ma bin joj quotes.
 * Ila kan quote ma mseddch, katrejje3 NULL.
 *
 * @param input String li fiha l quoted string
 * @param i Pointer l position fash ghadin nbdaw (b3d l quote),
 *          kat7ett 'i' f position b3d quote li kherat
 * @param quote Char dyal quote ('"' wla '\'')
 * @return char* String jdida li fras ma bin quotes, wla NULL ila kan quote
 *               ma mseddch. Khassek dir free 3la string li treturnat.
 */
char	*extract_quoted_string(char *input, int *i, char quote)
{
	int		start;
	int		len;
	char	*str;

	start = *i;
	len = 0;

	while (input[*i] && input[*i] != quote)
	{
		(*i)++;
		len++;
	}
	// handles Unclosed quote \\ fach kat7el chi " o ma katsedhach khass treturna NULL
	if (input[*i] != quote)
		return (NULL);
	// Extract the quoted string (without quotes)
	str = ft_substr(input, start, len);
	(*i)++;
	return (str);
}



//Extract an operator token from input // katjbed OPEARATERS tokens
// char	*extract_operator(char *input, int *i, t_token_type *type)
// {
// 	char	*operator;

// 	if (input[*i] == '>' && input[*i + 1] == '>')
// 	{
// 		operator = ft_strdup(">>");
// 		*type = TOKEN_REDIR_APPEND;
// 		(*i) += 2;
// 	}
// 	else if (input[*i] == '<' && input[*i + 1] == '<')
// 	{
// 		operator = ft_strdup("<<");
// 		*type = TOKEN_HEREDOC;
// 		(*i) += 2;
// 	}
// 	else if (input[*i] == '>')
// 	{
// 		operator = ft_strdup(">");
// 		*type = TOKEN_REDIR_OUT;
// 		(*i)++;
// 	}
// 	else if (input[*i] == '<')
// 	{
// 		operator = ft_strdup("<");
// 		*type = TOKEN_REDIR_IN;
// 		(*i)++;
// 	}
// 	else if (input[*i] == '|')
// 	{
// 		operator = ft_strdup("|");
// 		*type = TOKEN_PIPE;
// 		(*i)++;
// 	}
// 	else
// 	{
// 		operator = NULL;
// 		*type = TOKEN_WORD;
// 	}
// 	return (operator);
// }
