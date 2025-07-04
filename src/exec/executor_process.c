/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_execution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 03:09:01 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/04 03:09:01 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

static void	setup_child_input(int input_fd)
{
	if (input_fd != 0)
	{
		if (dup2(input_fd, STDIN_FILENO) == -1)
			exit(1);
		close(input_fd);
	}
}

static void	setup_child_output(t_cmds *cmd, int *pipe_fds)
{
	if (cmd->next)
	{
		if (dup2(pipe_fds[WRITE_END], STDOUT_FILENO) == -1)
			exit(1);
		close(pipe_fds[READ_END]);
		close(pipe_fds[WRITE_END]);
	}
}

static void	execute_builtin_command(t_cmds *cmd, t_env *env)
{
	t_builtin_func	builtin;
	t_shell			temp_shell;

	builtin = get_builtin(cmd->args[0]);
	if (builtin)
	{
		temp_shell.env = env;
		temp_shell.exit_status = g_state.exit_status;
		exit(builtin(&temp_shell, cmd->args));
	}
}

static void	execute_external(t_cmds *cmd, t_env *env)
{
	char	*cmd_path;
	char	**env_array;

	cmd_path = get_command_path(cmd->args[0], env);
	if (!cmd_path)
	{
		ft_cmd_error(cmd->args[0], "command not found\n", 127);
		exit(127);
	}
	env_array = convert_env_to_array(env);
	if (!env_array)
	{
		free(cmd_path);
		exit(1);
	}
	execve(cmd_path, cmd->args, env_array);
	handle_execution_error(cmd_path, cmd->args[0]);
	free(cmd_path);
	free_env_array(env_array);
	exit(1);
}

void	ft_child_process(t_cmds *cmd, t_env *env, int input_fd, int *pipe_fds)
{
	setup_child_input(input_fd);
	setup_child_output(cmd, pipe_fds);
	if (handle_exec_redirections(cmd) != 0)
		exit(1);
	setup_child_signals();
	if (is_builtin(cmd->args[0]))
		execute_builtin_command(cmd, env);
	execute_external(cmd, env);
}
