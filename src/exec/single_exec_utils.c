/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_exec_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 00:36:35 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 03:48:44 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"
#include "../../includes/mini_shell.h"

int	execute_builtin_with_redirections(t_shell *shell, t_cmds *cmd)
{
	t_builtin_func	builtin;
	int				stdin_backup;
	int				stdout_backup;
	int				status;

	builtin = get_builtin(cmd->args[0]);
	if (!builtin)
		return (1);
	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (setup_redirections(cmd) != 0)
	{
		reset_redirections(stdin_backup, stdout_backup);
		return (1);
	}
	status = builtin(shell, cmd->args);
	reset_redirections(stdin_backup, stdout_backup);
	return (status);
}

static int	handle_child_process(t_shell *shell, t_cmds *cmd, pid_t pid)
{
	int	status;

	(void)shell;
	(void)cmd;
	setup_parent_waiting_signals();
	waitpid(pid, &status, 0);
	restore_signals();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			return (130);
		else if (WTERMSIG(status) == SIGQUIT)
			return (131);
		else
			return (128 + WTERMSIG(status));
	}
	return (1);
}

static int	prepare_and_fork_external_command(
	t_shell *shell, t_cmds *cmd, int *stdin_backup, int *stdout_backup)
{
	pid_t	pid;

	*stdin_backup = dup(STDIN_FILENO);
	*stdout_backup = dup(STDOUT_FILENO);
	if (setup_redirections(cmd) != 0)
	{
		reset_redirections(*stdin_backup, *stdout_backup);
		return (1);
	}
	pid = fork();
	if (pid < 0)
	{
		reset_redirections(*stdin_backup, *stdout_backup);
		ft_cmd_error(cmd->args[0], "Fork failed", 1);
		return (1);
	}
	if (pid == 0)
		execute_child_command(shell, cmd);
	reset_redirections(*stdin_backup, *stdout_backup);
	return (pid);
}

int	execute_external_command(t_shell *shell, t_cmds *cmd)
{
	int		stdin_backup;
	int		stdout_backup;
	int		pid;

	pid = prepare_and_fork_external_command(shell, cmd,
			&stdin_backup, &stdout_backup);
	if (pid == 1)
		return (1);
	if (pid > 0)
		return (handle_child_process(shell, cmd, pid));
	return (0);
}
