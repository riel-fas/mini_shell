/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:14:51 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:15:00 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

void	extend_cmd(t_cmd **node,
			int needed_capacity, t_shell *shell)
{
	if (needed_capacity > (*node)->cmd_capacity)
	{
		if (!cmd_array_res(node, needed_capacity * 2))
		{
			perror("Failed to resize cmd array");
			shell->exit_status = 1;
		}
	}
}

void	add_first_word(t_cmd **node, char **split_words,
		bool first_word_joins)
{
	char	*tmp;

	if (first_word_joins && (*node)->cmdsize > 0)
	{
		tmp = (*node)->cmd[(*node)->cmdsize - 1];
		(*node)->cmd[(*node)->cmdsize - 1] = ft_strjoin(tmp, split_words[0]);
		free(tmp);
	}
	else
		(*node)->cmd[(*node)->cmdsize++] = ft_strdup(split_words[0]);
}

void	add_splited_words(t_cmd **node,
			char **split_words, bool first_word_joins, t_shell *shell)
{
	int		i;
	int		words_count;
	int		needed_capacity;

	if (!split_words || !split_words[0])
		return ;
	words_count = 0;
	while (split_words[words_count])
		words_count++;
	needed_capacity = (*node)->cmdsize + words_count;
	if (first_word_joins && (*node)->cmdsize > 0)
		needed_capacity--;
	extend_cmd(node, needed_capacity, shell);
	add_first_word(node, split_words, first_word_joins);
	i = 1;
	while (split_words[i])
	{
		(*node)->cmd[(*node)->cmdsize++] = ft_strdup(split_words[i]);
		i++;
	}
}

void	free_splited_words(char **split_words)
{
	int	i;

	if (!split_words)
		return ;
	i = 0;
	while (split_words[i])
		free(split_words[i++]);
	free(split_words);
}

void	splited_words_handling(t_cmd **node,
			t_token *token_list, char *val, t_shell *shell)
{
	char	**split_words;
	bool	should_join;

	if (!val || !*val)
	{
		free(val);
		return ;
	}
	split_words = ft_split(val, " \t");
	if (!split_words)
	{
		free(val);
		perror("malloc failure in ft_split");
		shell->exit_status = 1;
		return ;
	}
	if (!split_words[0])
		return (free_splited_words(split_words), free(val));
	should_join = (token_list->current->prev != NULL
			&& token_list->current->prev->addspace == false);
	add_splited_words(node, split_words, should_join, shell);
	free_splited_words(split_words);
	free(val);
}
