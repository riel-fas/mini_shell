/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 03:14:12 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 03:48:16 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

int	create_pipe_safe(int *pipe_fds)
{
	if (pipe(pipe_fds) == -1)
	{
		ft_cmd_error(NULL, "pipe error\n", 1);
		g_state.exit_status = 1;
		return (0);
	}
	return (1);
}

static pid_t	fork_command_process(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		g_state.exit_status = 1;
		perror("minishell: fork");
		return (-1);
	}
	return (pid);
}

static int	execute_single(t_cmds *current, t_env *env, int input_fd,
		int *pipe_fds)
{
	pid_t	pid;

	pid = fork_command_process();
	if (pid == -1)
		return (0);
	if (pid == 0)
		ft_child_process(current, env, input_fd, pipe_fds);
	else
		ft_parent_process(&input_fd, pipe_fds, current);
	return (pid);
}

int	execution(t_cmds *commands, t_env *env)
{
	int		pipe_fds[2];
	int		input_fd;
	pid_t	last_pid;
	t_cmds	*current;

	if (!commands)
		return (0);
	input_fd = 0;
	current = commands;
	last_pid = -1;
	while (current)
	{
		if (current->next && !create_pipe_safe(pipe_fds))
			return (0);
		last_pid = execute_single(current, env, input_fd, pipe_fds);
		if (last_pid <= 0)
			return (0);
		current = current->next;
	}
	finish_execution(last_pid);
	return (1);
}

void	ft_parent_process(int *input_fd, int *pipe_fds, t_cmds *cmd)
{
	if (*input_fd != 0)
		close(*input_fd);
	if (cmd->next)
	{
		close(pipe_fds[WRITE_END]);
		*input_fd = pipe_fds[READ_END];
	}
}
