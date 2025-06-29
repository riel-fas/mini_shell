/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 21:10:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/29 16:15:00 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include <sys/types.h>
#include <unistd.h>

/**
 * @brief Kat extract variable name men position '$'
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

	// Single digit (positional parameters)
	if (ft_isdigit(input[*i]))
	{
		char digit_str[2] = {input[*i], '\0'};
		(*i)++;
		return (ft_strdup(digit_str));
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

	// Invalid variable name - don't advance index, return NULL
	return (NULL);
}

/**
 * @brief Katjbed l9ima dyal variable li bghina n expandiha
 */
static char	*get_var_value(char *var_name, t_env *env, int exit_status)
{
	char *value;

	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(exit_status));
	if (ft_strcmp(var_name, "$") == 0)
		return (ft_itoa(getpid()));
	if (ft_strcmp(var_name, "UID") == 0)
		return (ft_itoa(getuid()));
	if (ft_strcmp(var_name, "EUID") == 0)
		return (ft_itoa(geteuid()));

	// Positional parameters
	if (ft_strlen(var_name) == 1 && ft_isdigit(var_name[0]))
	{
		if (var_name[0] == '0')
			return (ft_strdup("minishell"));  // Shell name
		else
			return (ft_strdup(""));  // Other positional parameters are empty
	}

	// Regular environment variable
	value = get_env_value(env, var_name);
	if (value)
		return (ft_strdup(value));

	return (ft_strdup(""));  // Empty string if variable doesn't exist
}

/**
 * @brief CORRECTLY handles mixed quotes - preserves content in single quotes
 */
char	*expand_variables(char *str, t_env *env, int exit_status, int in_quotes)
{
	char	*result;
	char	*temp;
	char	*var_name;
	char	*var_value;
	int		i;
	int		in_single_quotes;
	int		in_double_quotes;

	(void)in_quotes; // Parameter not used in this implementation
	if (!str)
		return (NULL);

	result = ft_strdup("");
	i = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;

	while (str[i])
	{
		// Handle quote state tracking
		if (str[i] == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
			i++; // Skip the quote character itself
			continue;
		}
		else if (str[i] == '"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
			i++; // Skip the quote character itself
			continue;
		}
		// Handle variable expansion (only if not in single quotes)
		else if (str[i] == '$' && !in_single_quotes && str[i + 1] &&
				 (ft_isalpha(str[i + 1]) || str[i + 1] == '_' ||
				  str[i + 1] == '?' || str[i + 1] == '$' ||
				  ft_isdigit(str[i + 1])))
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
		}
		else
		{
			// Regular character - add to result
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
 * @brief Kat expandir tokens f lista
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
