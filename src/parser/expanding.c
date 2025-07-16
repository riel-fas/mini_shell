/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:09:17 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:09:27 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

void	expanding_exit(t_token **token, t_shell *shell)
{
	ssize_t	dollar_pos;
	char	*expanded;
	char	*temp;
	char	*temp2;
	char	*leak;

	dollar_pos = check_dollar((*token)->value);
	if ((*token)->value[dollar_pos + 1] == '?')
		expanded = ft_itoa(shell->exit_status);
	else if ((*token)->value[dollar_pos + 1] == '$')
		expanded = ft_itoa(getpid());
	else
		expanded = ft_strdup("");
	leak = (*token)->value;
	temp = str_join_free(ft_substr((*token)->value, 0, dollar_pos), expanded);
	temp2 = ft_substr((*token)->value, dollar_pos + 2,
			ft_strlen((*token)->value));
	(*token)->value = str_join_free(temp, temp2);
	free(expanded);
	free(leak);
	(*token)->variable = true;
	free(temp2);
}

void	expanding_norm(t_token **token, t_list *minienv, ssize_t dpos)
{
	char	*exp;
	char	*temp;
	char	*temp2;

	exp = expanding(token, minienv, dpos);
	temp = str_join_free(ft_substr((*token)->value, 0, dpos), exp);
	temp2 = remaining_por(token, dpos);
	free((*token)->value);
	(*token)->value = str_join_free(temp, temp2);
	free(temp2);
	(*token)->variable = true;
	free(exp);
}

void	variables_expanding(t_token **token, t_list *minienv, t_shell *shell)
{
	ssize_t	dpos;

	if ((*token)->type == SINGLE_QUOTE)
		return ;
	dpos = check_dollar((*token)->value);
	while (dpos != -1)
	{
		if (dollar_exp(token, dpos))
		{
			expanding_exit(token, shell);
			dpos = check_dollar((*token)->value);
			continue ;
		}
		expanding_norm(token, minienv, dpos);
		dpos = check_dollar((*token)->value);
	}
}

char	*env_getting(char *var, t_list *minienv)
{
	t_list	*tmp;

	tmp = minienv;
	if (tmp == NULL)
		return (NULL);
	while (tmp)
	{
		if (ft_strcmp(var, tmp->key) == 0)
			return (ft_strdup(tmp->value));
		tmp = tmp->next;
	}
	return (NULL);
}

char	*var_getting(char *var, t_list *minienv)
{
	char	*value;

	value = env_getting(var, minienv);
	if (!value)
	{
		value = ft_strdup("");
		return (value);
	}
	return (value);
}
