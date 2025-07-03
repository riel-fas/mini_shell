/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   robust_executor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 13:15:00 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 17:59:14 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"
#include "../../includes/executor.h"

/**
 * Execute a command with enhanced error handling and memory management
 * @param shell Shell context
 * @param cmd Command to execute
 * @return Exit status
 */
int	execute_command_robust(t_shell *shell, t_cmds *cmd)
{
	int		result;
	char	*cmd_path;
	char	**env_array;

	// Check for errors before executing
	result = detect_execution_errors(shell, cmd);
	if (result != 0)
		return (result);

	// Handle empty command
	if (!cmd->args || !cmd->args[0])
		return (handle_empty_command_redirections(cmd));

	// Check if it's a builtin
	if (is_builtin(cmd->args[0]))
		return (execute_builtin_with_redirections(shell, cmd));

	// Get command path
	cmd_path = get_command_path_enhanced(cmd->args[0], shell->env);
	if (!cmd_path)
		return (127); // Command not found

	// Fork process
	pid_t pid = fork();
	if (pid < 0)
	{
		free(cmd_path);
		return (ft_cmd_error("fork", "failed", 1), 1);
	}

	if (pid == 0)
	{
		// Child process
		setup_child_signals();

		// Set up redirections
		if (setup_redirections(cmd) != 0)
			exit(1);

		// Convert env to array for execve
		env_array = convert_env_to_array(shell->env);
		if (!env_array)
		{
			free(cmd_path);
			exit(1);
		}

		// Execute command
		execve(cmd_path, cmd->args, env_array);

		// If we get here, execve failed
		handle_execution_error(cmd_path, cmd->args[0]);

		// Clean up and exit
		free(cmd_path);
		free_str_array(env_array);
		exit(127);
	}

	// Parent process
	free(cmd_path);
	setup_parent_waiting_signals();

	// Wait for child
	int status;
	waitpid(pid, &status, 0);

	// Restore signals
	restore_signals();

	// Return exit status
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));

	return (1);
}

/**
 * Execute a pipeline with enhanced error handling and memory management
 * @param shell Shell context
 * @param commands Commands in the pipeline
 * @return Exit status of the last command
 */
int	execute_pipeline_robust(t_shell *shell, t_cmds *commands)
{
	int	cmd_count;
	int	error_code;

	// Count commands
	cmd_count = 0;
	t_cmds *temp = commands;
	while (temp)
	{
		cmd_count++;
		temp = temp->next;
	}

	// If only one command, use simple execution
	if (cmd_count == 1)
		return (execute_command_robust(shell, commands));

	// Check for errors before executing
	error_code = detect_execution_errors(shell, commands);
	if (error_code != 0)
		return (error_code);

	// Execute pipeline
	return (execute_pipeline_enhanced(shell, commands));
}

/**
 * Main execution function with robust error handling
 * @param shell Shell context
 * @param commands Commands to execute
 * @return Exit status
 */
int	execute_commands_robust(t_shell *shell, t_cmds *commands)
{
	int	result;

	if (!commands)
		return (0);

	// Process heredocs first
	if (!process_heredocs_after_parsing(commands))
		return (g_exit_status);

	// Execute commands
	if (!commands->next)
		result = execute_command_robust(shell, commands);
	else
		result = execute_pipeline_robust(shell, commands);

	return (result);
}
