/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:16:30 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:16:43 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

static void	handle_heredoc_process(int pid,
			t_heredoc_ctx *ctx, t_parent_ctx *parent_ctx_struct)
{
	if (pid == 0)
		open_here_doc_ctx(ctx);
	else
	{
		parent_ctx(parent_ctx_struct);
		restore_signals();
	}
}

static int	handle_heredoc_type(t_redi *redi, t_list *env, t_shell *shell)
{
	int				pid;
	int				status;
	int				fd_pipe[2];
	t_heredoc_ctx	ctx;
	t_parent_ctx	parent_ctx_struct;

	status = 0;
	if (pipe(fd_pipe) == -1)
		command_error(NULL, "pipe failure", 1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	ctx.delimiter = NULL;
	ctx.fd_pipe = fd_pipe;
	ctx.heredoc = redi;
	ctx.env = env;
	ctx.shell = shell;
	parent_ctx_struct.fd_pipe = fd_pipe;
	parent_ctx_struct.pid = pid;
	parent_ctx_struct.status = &status;
	parent_ctx_struct.in = redi;
	parent_ctx_struct.shell = shell;
	handle_heredoc_process(pid, &ctx, &parent_ctx_struct);
	return (1);
}

int	handel_heredoc(t_redi *redi, t_list *env, t_shell *shell)
{
	if (redi == NULL)
		return (1);
	setup_parent_heredoc_signals();
	if (redi->type == HEREDOC)
		return (handle_heredoc_type(redi, env, shell));
	return (1);
}

int	here_doc_check(t_cmd *shell, t_list *env, t_shell *sh)
{
	t_cmd	*tmp;
	t_redi	*redi;

	tmp = shell;
	sh->exit_status = 0;
	if (!tmp)
		return (1);
	while (tmp)
	{
		init_redi_file(tmp);
		redi = tmp->redirections;
		tmp->origin_stdin = -1;
		tmp->origin_stdout = -1;
		while (redi)
		{
			if (handel_heredoc(redi, env, sh) == -1 || sh->exit_status == 1)
				return (0);
			redi = redi->next;
		}
		tmp = tmp->next;
	}
	return (1);
}
