/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:46:23 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:46:29 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

void	wait_for_children(int *status, pid_t last_cmd_pid, t_shell *shell)
{
	int		last_cmd_status;
	pid_t	waited_pid;

	last_cmd_status = 0;
	waited_pid = 1;
	while (waited_pid > 0)
	{
		waited_pid = wait(status);
		if (waited_pid == -1)
			continue ;
		if (waited_pid == last_cmd_pid)
		{
			if (WIFEXITED(*status))
				last_cmd_status = WEXITSTATUS(*status);
			else if (WIFSIGNALED(*status))
			{
				if (WTERMSIG(*status) == SIGPIPE)
					last_cmd_status = 0;
				else
					last_cmd_status = 128 + WTERMSIG(*status);
			}
		}
	}
	shell->exit_status = last_cmd_status;
}

static int	execution_fork_and_handle(t_exec_ctx *ctx)
{
	pid_t		pid;
	t_child_ctx	child_ctx;

	pid = fork();
	if (pid == -1)
		return (ctx->shell->exit_status = 1, perror("minishell: fork"), 0);
	child_ctx.current_cmd = ctx->current_cmd;
	child_ctx.env = ctx->env;
	child_ctx.tmp_in = *(ctx->tmp_in);
	child_ctx.p_fd = ctx->pip_fd;
	child_ctx.shell = ctx->shell;
	if (pid == 0)
		ft_child_ctx(&child_ctx);
	else
	{
		*(ctx->last_cmd_pid) = pid;
		parent_processing(ctx->tmp_in, ctx->pip_fd, ctx->current_cmd);
	}
	return (1);
}

static int	execution_loop(t_cmd *current_cmd,
				t_list *env, t_shell *shell, pid_t *last_cmd_pid)
{
	int			pip_fd[2];
	int			tmp_in;
	t_exec_ctx	ctx;

	tmp_in = 0;
	*last_cmd_pid = 0;
	while (current_cmd)
	{
		if (current_cmd->next)
			if (!create_pipe(pip_fd, shell))
				return (0);
		ctx.current_cmd = current_cmd;
		ctx.env = env;
		ctx.shell = shell;
		ctx.tmp_in = &tmp_in;
		ctx.pip_fd = pip_fd;
		ctx.last_cmd_pid = last_cmd_pid;
		if (!execution_fork_and_handle(&ctx))
			return (0);
		current_cmd = current_cmd->next;
	}
	return (1);
}

int	execution(t_cmd *current_cmd, t_list *env, t_shell *shell)
{
	pid_t	last_cmd_pid;

	if (!execution_loop(current_cmd, env, shell, &last_cmd_pid))
		return (0);
	return (finish_exec(last_cmd_pid, shell), 1);
}
