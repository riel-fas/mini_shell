/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 19:22:32 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 19:41:26 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"

static int	check_var_conditions(char *str, int i)
{
	return (str[i] == '$' && str[i + 1]
		&& (ft_isalpha(str[i + 1]) || str[i + 1] == '_'
			|| str[i + 1] == '?' || str[i + 1] == '$'
			|| ft_isdigit(str[i + 1])));
}

int	contains_unquoted_variables(char *str)
{
	int		i;
	int		in_quotes;
	char	quote_char;

	if (!str)
		return (0);
	i = 0;
	in_quotes = 0;
	quote_char = 0;
	while (str[i])
	{
		if (!in_quotes && (str[i] == '"' || str[i] == '\''))
		{
			in_quotes = 1;
			quote_char = str[i];
		}
		else if (in_quotes && str[i] == quote_char)
			in_quotes = 0;
		else if (!in_quotes && check_var_conditions(str, i))
			return (1);
		i++;
	}
	return (0);
}

int	should_word_split(char *orig_val, char *exp_val)
{
	int	i;

	if (!orig_val || !exp_val)
		return (0);
	if (!contains_unquoted_variables(orig_val))
		return (0);
	i = 0;
	while (exp_val[i])
	{
		if (exp_val[i] == ' ' || exp_val[i] == '\t')
			return (1);
		i++;
	}
	return (0);
}

static void	process_heredoc_content(t_cmds *cmd, char *delimiter)
{
	t_heredoc_list	*new;
	t_heredoc_list	*temp;

	new = malloc(sizeof(t_heredoc_list));
	if (!new)
		return ;
	new->delimiter = ft_strdup(delimiter);
	new->next = NULL;
	if (!cmd->heredoc_list)
		cmd->heredoc_list = new;
	else
	{
		temp = cmd->heredoc_list;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
	if (cmd->heredoc_delimeter)
		free(cmd->heredoc_delimeter);
	cmd->heredoc_delimeter = ft_strdup(delimiter);
}

void	handle_redirections(t_cmds *cmd, t_token **tokens)
{
	t_token	*curr;

	curr = *tokens;
	if (!curr || !curr->next)
		return ;
	if (curr->type == TOKEN_REDIR_IN)
		cmd->input_file = ft_strdup(curr->next->value);
	else if (curr->type == TOKEN_REDIR_OUT)
	{
		cmd->output_file = ft_strdup(curr->next->value);
		cmd->append_node = 0;
	}
	else if (curr->type == TOKEN_REDIR_APPEND)
	{
		cmd->output_file = ft_strdup(curr->next->value);
		cmd->append_node = 1;
	}
	else if (curr->type == TOKEN_REDIR_READ_WRITE)
		cmd->rw_file = ft_strdup(curr->next->value);
	else if (curr->type == TOKEN_HEREDOC)
		process_heredoc_content(cmd, curr->next->value);
	*tokens = curr->next->next;
}
