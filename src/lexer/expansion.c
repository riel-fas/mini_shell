/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 21:10:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/20 19:07:36 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include <sys/types.h>
#include <unistd.h>

/**
 * @brief Kat extract variable name men position '$'
 *
 * Had fonction katbda men position b3d '$' o kat extract l variable name.
 * Variable names kanun alphanumeric characters w underscores.
 * Special cases: $? (exit status) w $$ (process ID)

 */
static char	*extract_var_name(char *input, int *i)
{
	int		start;
	int		len;

	start = *i;
	len = 0;

	// Special cases: $? and $$
	if (input[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	if (input[*i] == '$')
	{
		(*i)++;
		return (ft_strdup("$"));
	}

	// Regular variable name: letters, digits, underscore
	if (ft_isalpha(input[*i]) || input[*i] == '_')
	{
		while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		{
			(*i)++;
			len++;
		}
		return (ft_substr(input, start, len));
	}

	// Invalid variable name
	return (NULL);
}

/**
 * @brief Katjbed l9ima dyal variable li bghina n expandiha
 *
 * Had fonction kat7awel variable name l value dyalo.
 * Special cases: ? (exit status), $ (process ID)
 * Regular variables: men environment
 */
static char	*get_var_value(char *var_name, t_env *env, int exit_status)
{
	char *value;

	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(exit_status));
	if (ft_strcmp(var_name, "$") == 0)
		return (ft_itoa(getpid()));

	// Regular environment variable
	value = get_env_value(env, var_name);
	if (value)
		return (ft_strdup(value));

	return (ft_strdup(""));  // Empty string if variable doesn't exist
}

/**
 * @brief Kat expandir variables f string wa7d
 *
 * Had fonction katdour 3la string o kat9elleb 3la '$' characters.
 * Ila l9at '$', kat extract variable name o kat replace biha value dyalo.
 * Katrejje3 string jdid fiha all expansions.
 */
char	*expand_variables(char *str, t_env *env, int exit_status, int in_quotes)
{
	char	*result;
	char	*temp;
	char	*var_name;
	char	*var_value;
	int		i;

	if (!str)
		return (NULL);

	// If in single quotes, no expansion
	if (in_quotes == 1)  // 1 = single quotes
		return (ft_strdup(str));

	result = ft_strdup("");
	i = 0;

	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			i++; // Skip the '$'
			var_name = extract_var_name(str, &i);
			if (var_name)
			{
				var_value = get_var_value(var_name, env, exit_status);
				temp = ft_strjoin(result, var_value);
				free(result);
				free(var_name);
				free(var_value);
				result = temp;
			}
			else
			{
				// Invalid variable, keep the '$'
				temp = ft_strjoin(result, "$");
				free(result);
				result = temp;
			}
		}
		else
		{
			// Regular character, append to result
			char char_str[2] = {str[i], '\0'};
			temp = ft_strjoin(result, char_str);
			free(result);
			result = temp;
			i++;
		}
	}

	return (result);
}

/**
 * @brief Kat expandir variables f kol tokens
 *
 * Had fonction katdour 3la kol token f linked list o kat expandir
 * variables li fihom. Katbeddel value dyal token b expanded version.
 * Single quotes: no expansion
 * Double quotes: allow expansion
 * Regular words: allow expansion
 */
void	expand_tokens(t_token *tokens, t_env *env, int exit_status)
{
	t_token	*current;
	char	*expanded;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			// Regular word - allow expansion
			expanded = expand_variables(current->value, env, exit_status, 0);
			if (expanded)
			{
				free(current->value);
				current->value = expanded;
			}
		}
		else if (current->type == TOKEN_DOUBLE_QUOTED)
		{
			// Double quotes - allow expansion
			expanded = expand_variables(current->value, env, exit_status, 2);
			if (expanded)
			{
				free(current->value);
				current->value = expanded;
			}
		}
		// TOKEN_SINGLE_QUOTED - no expansion needed
		current = current->next;
	}
}
