/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 21:10:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/01 02:46:03 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"

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
	if (ft_isdigit(input[*i]))
	{
		char digit_str[2] = {input[*i], '\0'};
		(*i)++;
		return (ft_strdup(digit_str));
	}
	if (ft_isalpha(input[*i]) || input[*i] == '_')
	{
		while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		{
			(*i)++;
			len++;
		}
		return (ft_substr(input, start, len));
	}
	return (NULL);
}


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
	value = get_env_value(env, var_name);
	if (value)
		return (ft_strdup(value));

	return (ft_strdup(""));  // Empty string if variable doesn't exist
}


char	*expand_variables(char *str, t_env *env, int exit_status, int in_quotes)
{
	char	*result;
	int		len;
	int		i;
	int		j;
	int		in_single_quotes;
	int		in_double_quotes;
	int		result_size;

	(void)in_quotes;
	if (!str)
		return (NULL);

	len = ft_strlen(str);
	// Allocate much more space to handle long variable expansions
	result_size = len * 10 + 1024;  // Be very generous with allocation
	result = malloc(result_size);
	if (!result)
		return (NULL);

	i = 0;
	j = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;

	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
			i++; // Skip quote, don't copy it
		}
		else if (str[i] == '"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
			i++; // Skip quote, don't copy it
		}
		else if (str[i] == '$' && !in_single_quotes && str[i + 1])
		{
			if (str[i + 1] == '"' || str[i + 1] == '\'')
			{
				// Handle $"..." and $'...' - skip the $ and treat as regular quotes
				i++; // Skip the $
				// The quote will be handled in the next iteration
			}
			else if (ft_isalpha(str[i + 1]) || str[i + 1] == '_' ||
					 str[i + 1] == '?' || str[i + 1] == '$' ||
					 ft_isdigit(str[i + 1]))
			{
				// Variable expansion
				char *var_name;
				char *var_value;

				i++; // Skip '$'
				var_name = extract_var_name(str, &i);
				if (var_name)
				{
					var_value = get_var_value(var_name, env, exit_status);
					if (var_value)
					{
						int val_len = ft_strlen(var_value);
						// Safety check: ensure we don't overflow the buffer
						if (j + val_len + 1 < result_size)
						{
							ft_memcpy(result + j, var_value, val_len);
							j += val_len;
						}
						else
						{
							// Buffer would overflow - truncate or handle error
							// For safety, we'll truncate to fit
							int space_left = result_size - j - 1;
							if (space_left > 0)
							{
								ft_memcpy(result + j, var_value, space_left);
								j += space_left;
							}
						}
						free(var_value);
					}
					free(var_name);
				}
			}
			else
			{
				// Invalid variable name, copy $ literally
				if (j + 1 < result_size)
					result[j++] = str[i++];
				else
					i++; // Skip if no space
			}
		}
		else
		{
			// Copy regular character
			if (j + 1 < result_size)
				result[j++] = str[i++];
			else
				i++; // Skip if no space
		}
	}
	result[j] = '\0';

	// Reallocate to exact size
	char *final_result = ft_strdup(result);
	free(result);
	return (final_result);
}



void	expand_tokens(t_token *tokens, t_env *env, int exit_status)
{
	t_token	*current;
	char	*expanded;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			expanded = expand_variables(current->value, env, exit_status, 0);
			if (expanded)
			{
				free(current->value);
				current->value = expanded;
			}
		}
		current = current->next;
	}
}
