/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:18:05 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:18:12 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

int	open_here_doc_ctx(t_heredoc_ctx *ctx)
{
	close(ctx->fd_pipe[0]);
	ctx->delimiter = NULL;
	setup_heredoc_signals();
	ctx->delimiter = my_strjoin(ctx->heredoc->file, "\n");
	if (!ctx->delimiter)
		command_error(NULL, "malloc failure\n", 1);
	read_line_by_line(ctx);
	free(ctx->delimiter);
	exit(0);
}

void	parent_ctx(t_parent_ctx *ctx)
{
	close(ctx->fd_pipe[1]);
	waitpid(ctx->pid, ctx->status, 0);
	ctx->shell->exit_status = WEXITSTATUS(*ctx->status);
	if (ctx->shell->exit_status == 0 && ctx->in->is_last)
		ctx->in->heredoc_fd = ctx->fd_pipe[0];
	else
	{
		close(ctx->fd_pipe[0]);
		ctx->in->heredoc_fd = -1;
	}
}
