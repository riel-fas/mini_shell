/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 19:25:09 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 19:25:13 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"

t_cmds	*init_new_command(t_cmds **head, t_cmds **tail)
{
	t_cmds	*cmd;

	cmd = malloc(sizeof(t_cmds));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->rw_file = NULL;
	cmd->append_node = 0;
	cmd->heredoc_delimeter = NULL;
	cmd->heredoc_list = NULL;
	cmd->heredoc_fd = -1;
	cmd->next = NULL;
	if (!*head)
		*head = cmd;
	else
		(*tail)->next = cmd;
	*tail = cmd;
	return (cmd);
}

void	add_arg_to_cmd(t_cmds *cmd, char *value)
{
	int		count;
	char	**new_args;

	count = 0;
	while (cmd->args && cmd->args[count])
		count++;
	new_args = ft_realloc(cmd->args, sizeof(char *) * (count + 1),
			sizeof(char *) * (count + 2));
	if (!new_args)
		return ;
	cmd->args = new_args;
	cmd->args[count] = ft_strdup(value);
	cmd->args[count + 1] = NULL;
}

static int	count_words(char *value)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (value[i])
	{
		while (value[i] && (value[i] == ' ' || value[i] == '\t'))
			i++;
		if (value[i])
		{
			count++;
			while (value[i] && value[i] != ' ' && value[i] != '\t')
				i++;
		}
	}
	return (count);
}

static char	**extract_words(char *value, int word_count)
{
	char	**words;
	int		i;
	int		start;
	int		wc;

	words = malloc(sizeof(char *) * (word_count + 1));
	if (!words)
		return (NULL);
	i = 0;
	wc = 0;
	while (value[i] && wc < word_count)
	{
		while (value[i] && (value[i] == ' ' || value[i] == '\t'))
			i++;
		start = i;
		while (value[i] && value[i] != ' ' && value[i] != '\t')
			i++;
		words[wc++] = ft_substr(value, start, i - start);
	}
	words[wc] = NULL;
	return (words);
}

void	add_split_args_to_cmd(t_cmds *cmd, char *value)
{
	char	**words;
	int		word_count;
	int		i;

	if (!cmd || !value || !*value)
		return ;
	word_count = count_words(value);
	if (word_count == 0)
		return ;
	words = extract_words(value, word_count);
	if (!words)
		return ;
	i = 0;
	while (words[i])
	{
		add_arg_to_cmd(cmd, words[i]);
		free(words[i++]);
	}
	free(words);
}
