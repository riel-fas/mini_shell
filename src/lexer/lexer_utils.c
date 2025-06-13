/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 19:15:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/13 12:58:21 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"

/**
 * @brief Katkhle9 token jdid
 *
 * Had fonction katkhle9 t_token jdid o kat3mro b value o type li 3tinaha.
 * Katdir malloc l structure o katdir strdup l value bash tkhebbi copy men value.
 * Ila kan chi moshkil f malloc, katrejje3 NULL. Darouri tkhelli next = NULL
 * bash nwejdo listi mezyana dyal tokens.
 *
 * @param value String dyal token (kelma, operator, etc.)
 * @param type Type dyal token (TOKEN_WORD, TOKEN_PIPE, etc.)
 * @return t_token* Token jdid, wla NULL ila kan moshkil f malloc
 */
t_token	*create_token(char *value, t_token_type type)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	return (token);
}

/**
 * @brief Katzid token f lekher dyal list
 *
 * Had fonction katzid new_token f lekher dyal lista dyal tokens.
 * Kat check 3la NULL pointers. Ila kanet lista khawya, kat7ett
 * new_token howa l'awel. Sinon, katdour 3la lista 7ta tlqa lekher
 * dyal tokens o katzid new_token f next dyalo.
 *
 * @param tokens Pointer l pointer dyal l'awel dyal lista (double pointer
 *              bach n9edro nbedlo l'awel dyal lista ila khass l'amr)
 * @param new_token Token jdid li bghina nzido f list
 */
void	add_token(t_token **tokens, t_token *new_token)
{
	t_token	*current;

	if (!tokens || !new_token)
		return;
	if (!*tokens)
	{
		*tokens = new_token;
		return;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

/**
 * @brief Katkhwi o kat7err memory dyal tokens kolhom
 *
 * Had fonction katdour 3la lista dyal tokens o katmsseh kolchi.
 * Kat7err l'memory dyal kola token (structure) o value dyalo.
 * Darouri nste3mlo had fonction melli kansaliw m3a lista dyal tokens
 * bach maykunch 3endna memory leak.
 *
 * @param tokens L'awel token f lista
 */
void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*temp;

	current = tokens;
	while (current)
	{
		temp = current;
		current = current->next;
		free(temp->value);
		free(temp);
	}
}
