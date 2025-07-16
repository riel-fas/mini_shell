/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:09:46 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 01:36:18 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

bool	dollar_exp(t_token **token, ssize_t dollar_pos)
{
	return ((*token)->value[dollar_pos + 1] == '$' || (*token)->value[dollar_pos
			+ 1] == '?' || ft_isdigit((*token)->value[dollar_pos + 1]));
}

char	*expanding(t_token **token, t_list *minienv, ssize_t dollar_pos)
{
	char	*var;
	char	*expanded;

	var = ft_substr((*token)->value, dollar_pos + 1,
			var_size((*token)->value + dollar_pos + 1));
	expanded = var_getting(var, minienv);
	free(var);
	return (expanded);
}

char	*remaining_por(t_token **token, ssize_t dollar_pos)
{
	return (ft_substr((*token)->value, dollar_pos
			+ var_size((*token)->value + dollar_pos + 1) + 1,
			ft_strlen((*token)->value)));
}

char	*str_join_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

char	**freeall(char **p, size_t x)
{
	while (x > 0)
		free(p[--x]);
	free(p);
	return (NULL);
}
