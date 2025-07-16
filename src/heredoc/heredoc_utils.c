/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:17:46 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:17:52 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

static t_redi	*get_last_input(t_redi *redi)
{
	t_redi	*last;
	t_redi	*tmp;

	last = NULL;
	tmp = redi;
	while (tmp)
	{
		tmp->tmp_fd = -1;
		tmp->is_last = false;
		tmp->heredoc_fd = -1;
		tmp->content = NULL;
		if (tmp->type == INPUT || tmp->type == HEREDOC)
			last = tmp;
		tmp = tmp->next;
	}
	return (last);
}

static t_redi	*get_last_output(t_redi *redi)
{
	t_redi	*last;

	last = NULL;
	while (redi)
	{
		redi->tmp_fd = -1;
		redi->is_last = false;
		redi->heredoc_fd = -1;
		redi->content = NULL;
		if (redi->type == OUTPUT || redi->type == APPEND)
			last = redi;
		redi = redi->next;
	}
	return (last);
}

void	init_redi_file(t_cmd *shell)
{
	t_redi	*last_input;
	t_redi	*last_output;
	t_cmd	*tmp;

	tmp = shell;
	last_input = NULL;
	last_output = NULL;
	if (!tmp)
		return ;
	while (tmp)
	{
		last_input = get_last_input(tmp->redirections);
		last_output = get_last_output(tmp->redirections);
		if (last_input)
			last_input->is_last = true;
		if (last_output)
			last_output->is_last = true;
		tmp = tmp->next;
	}
}

int	handle_heredoc_break(char *line, char *delimiter, t_shell *shell)
{
	if (line == NULL)
	{
		write(1, "\n", 1);
		return (1);
	}
	if (ft_strncmp(line, delimiter, ft_strlen(line)) == 0)
	{
		free(line);
		line = NULL;
		shell->exit_status = 0;
		return (1);
	}
	return (0);
}

void	read_line_by_line(t_heredoc_ctx *ctx)
{
	char	*line;

	line = NULL;
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0);
		if (handle_heredoc_break(line, ctx->delimiter, ctx->shell))
			break ;
		if (ctx->heredoc->expand)
			expanding_var_char(&line, ctx->env, ctx->shell);
		if (ctx->heredoc->is_last)
			ctx->heredoc->content = my_strjoin(ctx->heredoc->content, line);
		free(line);
		line = NULL;
	}
	if (ctx->heredoc->is_last)
	{
		write(ctx->fd_pipe[1], ctx->heredoc->content,
			ft_strlen(ctx->heredoc->content));
	}
	free(ctx->heredoc->content);
	ctx->heredoc->content = NULL;
	close(ctx->fd_pipe[1]);
}
