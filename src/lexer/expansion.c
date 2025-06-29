/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 21:10:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/06/29 17:40:32 by marvin           ###   ########.fr       */
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


static char	*process_special_quoting(char *str, int *i)
{
	char	*result;
	char	quote_type;
	int		start;
	int		len;

	// Skip the $ and get the quote type
	(*i)++; // Skip $
	quote_type = str[*i];
	(*i)++; // Skip opening quote
	start = *i;
	len = 0;

	while (str[*i] && str[*i] != quote_type)
	{
		(*i)++;
		len++;
	}
	if (str[*i] == quote_type)
		(*i)++; // Skip closing quote
	if (len == 0)
		result = ft_strdup("");
	else
		result = ft_substr(str, start, len);

	return (result);
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
		else if (str[i] == '$' && (str[i + 1] == '\'' || str[i + 1] == '"') &&
				 !in_single_quotes && !in_double_quotes)
		{
			var_value = process_special_quoting(str, &i);
			temp = ft_strjoin(result, var_value);
			free(result);
			free(var_value);
			result = temp;
		}
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
			char char_str[2] = {str[i], '\0'};
			temp = ft_strjoin(result, char_str);
			free(result);
			result = temp;
			i++;
		}
	}
	return (result);
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
		else if (current->type == TOKEN_DOUBLE_QUOTED)
		{
			expanded = expand_variables(current->value, env, exit_status, 2);
			if (expanded)
			{
				free(current->value);
				current->value = expanded;
			}
		}
		current = current->next;
	}
}
