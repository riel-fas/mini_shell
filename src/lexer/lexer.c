/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:26:09 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/13 12:58:26 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"

//Tokenize input string into a list of tokens // kanredo input 3la chkel list dyal tokens
// t_token	*tokenize(char *input)
// {
// 	t_token			*tokens;
// 	t_token			*new_token;
// 	int				i;
// 	char			*value;
// 	t_token_type	type;

// 	if (!input)
// 		return (NULL);
// 	tokens = NULL;
// 	i = 0;
// 	while (input[i])
// 	{
// 		while (is_whitespace(input[i]))
// 			i++;
// 		if (input[i] == '\0')
// 			break;
// 		if (is_operator(input[i]))
// 		{
// 			value = extract_operator(input, &i, &type);
// 			if (!value)
// 			{
// 				free_tokens(tokens);
// 				return (NULL);
// 			}
// 			new_token = create_token(value, type);
// 			free(value);
// 		}
// 		else if (is_quote(input[i]))
// 		{
// 			char quote = input[i++]; // Save quote type and move past it
// 			value = extract_quoted_string(input, &i, quote);
// 			if (!value)
// 			{
// 				free_tokens(tokens);
// 				return (NULL);
// 			}
// 			new_token = create_token(value, TOKEN_WORD);
// 			free(value);
// 		}
// 		else
// 		{
// 			value = extract_word(input, &i);
// 			if (!value)
// 			{
// 				free_tokens(tokens);
// 				return (NULL);
// 			}
// 			new_token = create_token(value, TOKEN_WORD);
// 			free(value);
// 		}
// 		if (!new_token)
// 		{
// 			free_tokens(tokens);
// 			return (NULL);
// 		}
// 		add_token(&tokens, new_token);
// 	}
// 	return (tokens);
// }

/**
 * @brief Kat créyi token jdid o kat handli l errors
 *
 * Had fonction d'aide katstakhdem bash tkhebbina code repetitif dyal creation
 * dyal tokens. Kat check wach 'value' machi NULL, katkhle9 token jdid b
 * create_token, o kat free l'value men be3dma tkhdem bih. Ila kan chi error,
 * kat free gaa3 tokens li deja kay'no o katrejje3 NULL.
 *
 * @param value String dyal value dyal token (kat free-ih had fonction)
 * @param type Type dyal token (TOKEN_WORD, TOKEN_PIPE, etc.)
 * @param tokens Lista dyal tokens li deja kaynin (bach ila kan error nfreewhom)
 * @return t_token* Token jdid ila daz koulchi mezyan, NULL ila kan chi moshkil
 */
static t_token	*handle_token_creation(char *value, t_token_type type,
				t_token *tokens)
{
	t_token	*new_token;

	if (!value)
	{
		free_tokens(tokens);
		return (NULL);
	}
	new_token = create_token(value, type);
	free(value);
	if (!new_token)
	{
		free_tokens(tokens);
		return (NULL);
	}
	return (new_token);
}

/**
 * @brief Katjbed value dyal token men input string
 *
 * Had fonction kat7aded ashmon type dyal token kayn f position 'i'
 * o kat extract l'value dyalo. Ila kan operator (|, <, >, etc.) kat3ayet
 * l extract_operator. Ila kan quote (', ") kattfayyet l quote o kat3ayet
 * l extract_quoted_string. F ay 7ala okhra, katstakhdem extract_word.
 * Katbeddel pointer 'i' o 'type' bash n3erfo ashno l9ina.
 *
 * @param input String li ghadi n9ellbo fiha 3la token
 * @param i Pointer l position fash ghadin n9ellbo, kaytwella position jdida
 * @param type Pointer l variable li ghadi n7ettu fiha type dyal token
 * @return char* String dyal value dyal token, khassek dir liha free men be3d
 */
static char	*get_token_value(char *input, int *i, t_token_type *type)
{
	if (is_operator(input[*i]))
		return (extract_operator(input, i, type));
	else if (is_quote(input[*i]))
	{
		char quote = input[(*i)++];
		*type = TOKEN_WORD;
		return (extract_quoted_string(input, i, quote));
	}
	else
	{
		*type = TOKEN_WORD;
		return (extract_word(input, i));
	}
}

/**
 * @brief Kat7ewel string l lista dyal tokens (lexical analysis)
 *
 * Had fonction principale dyal lexer. Katlexxi l'input string o kat7ewlo
 * l wasted lista dyal tokens. Katdour 3la kol character f input, katfayet
 * l whitespace, kat extract token values b get_token_value, o katkhle9 o
 * tzid tokens jdad l lista. Kola token 3endo type o value.
 *
 * Ila kan chi moshkil f khla9 tokens, katrejje3 NULL.
 *
 * @param input String dyal command line li bghina nlexiw
 * @return t_token* Ras dyal lista dyal tokens, NULL ila kant input khawya wla chi error
 */
t_token	*tokenize(char *input)
{
	t_token			*tokens;
	t_token			*new_token;
	int				i;
	char			*value;
	t_token_type	type;

	if (!input)
		return (NULL);
	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (is_whitespace(input[i]))
			i++;
		if (input[i] == '\0')
			break;
		value = get_token_value(input, &i, &type);
		new_token = handle_token_creation(value, type, tokens);
		if (!new_token)
			return (NULL);
		add_token(&tokens, new_token);
	}
	return (tokens);
}

/**
 * @brief Fonction dyal debugging: kataffichi tokens kamlin
 *
 * Had fonction katkhdem ghir bash nshoofo tokens li l9ina. Kataffichi type o
 * value dyal kol token. Type kay affichiw f format lisible (WORD, PIPE, etc.).
 * Katstakhdem f debugging bash nshoofo wach lexer kheddama mezyan.
 *
 * @param tokens Linked list dyal tokens li bghina naffichiwha
 */
void	print_tokens(t_token *tokens)
{
	t_token	*current;
	int		i;

	if (!tokens)
	{
		printf("No tokens to print\n");
		return;
	}

	current = tokens;
	i = 0;

	printf("\n--- Tokens ---\n");
	while (current)
	{
		printf("Token %d: Type = ", i);

		// Print token type as string
		switch (current->type)
		{
			case TOKEN_WORD:
				printf("WORD");
				break;
			case TOKEN_PIPE:
				printf("PIPE");
				break;
			case TOKEN_REDIR_IN:
				printf("REDIR_IN");
				break;
			case TOKEN_REDIR_OUT:
				printf("REDIR_OUT");
				break;
			case TOKEN_REDIR_APPEND:
				printf("REDIR_APPEND");
				break;
			case TOKEN_HEREDOC:
				printf("HEREDOC");
				break;
			case TOKEN_END_OF_INPUT:
				printf("END_OF_INPUT");
				break;
			default:
				printf("UNKNOWN");
		}

		printf(", Value = \"%s\"\n", current->value);

		current = current->next;
		i++;
	}
	printf("--------------\n");
}
