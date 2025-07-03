/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   advanced_single_exec.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 14:30:00 by riad              #+#    #+#             */
/*   Updated: 2025/07/03 14:30:00 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

extern int	g_exit_status;

/*
 * Executes a single command with proper builtin and redirection handling
 * Handles both builtin commands and external commands
 */
int	execute_single_command_advanced(t_shell *shell, t_cmds *cmd)
{
	// Handle empty command with only redirections
	if (!cmd->args || !cmd->args[0])
		return (handle_empty_command_redirections(cmd));

	// Handle special commands
	if (ft_strcmp(cmd->args[0], "!") == 0 && !cmd->args[1])
		return (1);

	// Handle builtin commands
	if (is_builtin(cmd->args[0]))
		return (execute_builtin_with_redirections(shell, cmd));

	// Handle external commands
	return (execute_external_command(shell, cmd));
}

/*
 * Handles commands that have no arguments but may have redirections
 * Processes redirections without executing any command
 */
int	handle_empty_command_redirections(t_cmds *cmd)
{
	int	stdin_backup;
	int	stdout_backup;
	int	status;

	if (!cmd->input_file && !cmd->output_file && !cmd->rw_file
		&& !cmd->heredoc_delimeter && cmd->heredoc_fd < 0 && !cmd->heredoc_list)
		return (0);

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);

	// Redirect stdout to /dev/null to suppress output
	int devnull = open("/dev/null", O_WRONLY);
	if (devnull >= 0)
	{
		dup2(devnull, STDOUT_FILENO);
		close(devnull);
	}

	status = setup_redirections(cmd);
	reset_redirections(stdin_backup, stdout_backup);

	if (status == 130) // SIGINT during heredoc
		return (130);

	return (status);
}

/*
 * Executes a builtin command with proper redirection handling
 * Builtins run in the current shell process with redirections
 */
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

/*
 * Executes an external command in a child process
 * Handles path resolution, redirections, and process management
 */
int	execute_external_command(t_shell *shell, t_cmds *cmd)
{
	pid_t	pid;
	int		status;
	int		stdin_backup;
	int		stdout_backup;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);

	if (setup_redirections(cmd) != 0)
	{
		reset_redirections(stdin_backup, stdout_backup);
		return (1);
	}

	pid = fork();
	if (pid < 0)
	{
		reset_redirections(stdin_backup, stdout_backup);
		ft_cmd_error(cmd->args[0], "Fork failed", 1);
		return (1);
	}
	else if (pid == 0)
	{
		// Child process
		execute_child_command(shell, cmd);
	}
	else
	{
		// Parent process
		setup_parent_waiting_signals();
		waitpid(pid, &status, 0);
		restore_signals();
		reset_redirections(stdin_backup, stdout_backup);

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
	return (0);
}

/*
 * Executes command in child process with proper error handling
 * Handles path resolution, environment setup, and execve
 */
void	execute_child_command(t_shell *shell, t_cmds *cmd)
{
	char			*cmd_path;
	char			**env_array;
	struct stat		file_stat;

	setup_child_signals();

	// Handle absolute/relative paths
	if (ft_strchr(cmd->args[0], '/'))
	{
		if (access(cmd->args[0], F_OK) == 0 &&
			stat(cmd->args[0], &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
		{
			ft_cmd_error(cmd->args[0], "Is a directory", 126);
			exit(126);
		}
	}

	cmd_path = get_command_path(cmd->args[0], shell->env);
	if (!cmd_path)
	{
		ft_cmd_error(cmd->args[0], "command not found", 127);
		exit(127);
	}

	env_array = convert_env_to_array(shell->env);
	if (!env_array)
	{
		free(cmd_path);
		ft_cmd_error(cmd->args[0], "Failed to create environment", 1);
		exit(1);
	}

	execve(cmd_path, cmd->args, env_array);

	// If execve fails
	handle_execution_error(cmd_path, cmd->args[0]);
	free(cmd_path);
	free_env_array(env_array);
	exit(1);
}

/*
 * Main single command execution entry point
 * Determines the type of command and calls appropriate handler
 */
int	execute_single_command(t_shell *shell, t_cmds *cmd)
{
	if (!cmd)
		return (0);

	return (execute_single_command_advanced(shell, cmd));
}
