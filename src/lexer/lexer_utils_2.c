/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:52:04 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/13 12:58:02 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"

/**
 * @brief Kat check wach character howa whitespace
 *
 * Had fonction bassita katstakhdam bash n3erfo wach character li
 * 3tinaha howa space, tab, newline wla carriage return. Katkheddmouha
 * f lexer bash n3erfo fin katsali kelma o fin katbda lokhra.
 * Katste3melha f extract_word bash n3erfo wach nwa9fo extraction.
 *
 * @param a Character li bghina ncheckyiwh
 * @return int 1 (TRUE) ila kan whitespace, 0 (FALSE) ila mankanch
 */
int	is_whitespace(char a)
{
	return (a == ' ' || a == '\t' || a == '\n' || a == '\r');
}

/**
 * @brief Kat check wach character howa operator
 *
 * Had fonction bassita katstakhdam bash n3erfo wach character li
 * 3tinaha howa pipe (|) wla redirection (<, >). Katstakhdam
 * f lexer bash n3erfo blast ntrtajmo character l token dyal operator.
 * Operators kaykono 3endhom m3na khas f shell o khass n traityiwhom
 * bshkel khas.
 *
 * @param a Character li bghina ncheckyiwh
 * @return int 1 (TRUE) ila kan operator, 0 (FALSE) ila mankanch
 */
int	is_operator(char a)
{
	return (a == '|' || a == '<' || a == '>');
}

/**
 * @brief Kat check wach character howa quote
 *
 * Had fonction bassita katstakhdam bash n3erfo wach character li
 * 3tinaha howa single quote (') wla double quote ("). Katstakhdam
 * f lexer bash n3erfo blast nbdaw string li katbda b quotes o n trtityiw
 * l content dyalha bshkel khas. L9ot3an (quotes) kaykono 3endhom
 * m3na khas f shell, 7it kataliminaw special characters.
 *
 * @param a Character li bghina ncheckyiwh
 * @return int 1 (TRUE) ila kan quote, 0 (FALSE) ila mankanch
 */
int	is_quote(char a)
{
	return (a == '\'' || a == '\"');
}
