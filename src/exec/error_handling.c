/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riad <riad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 12:19:00 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 19:16:43 by riad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"
#include "../../includes/executor.h"
#include "../../includes/error.h"

/**
 * Prints detailed error messages for command execution errors
 * @param cmd The command name that failed
 * @param error The error message or type
 * @param status The exit status to set
 *
 * Note: This function is actually defined in advanced_cmd_utils.c
 * We use the function declared there to avoid duplicate definition.
 */

/**
 * Handles specific exec error cases with appropriate messages
 * @param cmd_path The full path to the command that failed
 * @param cmd_name The original command name
 */
void	enhanced_exec_error(char *cmd_path, char *cmd_name)
{
	if (!cmd_path || !cmd_name)
		return;

	if (errno == ENOENT)
		ft_cmd_error(cmd_name, "No such file or directory", 127);
	else if (errno == EACCES)
		ft_cmd_error(cmd_name, "Permission denied", 126);
	else if (errno == ENOTDIR)
		ft_cmd_error(cmd_name, "Not a directory", 126);
	else if (errno == EISDIR)
		ft_cmd_error(cmd_name, "Is a directory", 126);
	else if (errno == E2BIG)
		ft_cmd_error(cmd_name, "Argument list too long", 126);
	else if (errno == ETXTBSY)
		ft_cmd_error(cmd_name, "Text file busy", 126);
	else if (errno == ENOMEM)
		ft_cmd_error(cmd_name, "Cannot allocate memory", 126);
	else
		ft_cmd_error(cmd_name, strerror(errno), 126); // Use strerror with ft_cmd_error
}

/**
 * Handles redirection errors with specific messages
 * @param file The file that caused the error
 * @param msg Additional error message
 * @param err Error number to set
 */
void	enhanced_redir_error(char *file, char *msg, int err)
{
	ft_putstr_fd("minishell: ", 2);
	if (file)
	{
		ft_putstr_fd(file, 2);
		ft_putstr_fd(": ", 2);
	}
	if (msg)
		ft_putstr_fd(msg, 2);
	else
		ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	g_exit_status = err;
}

/**
 * Handles ambiguous redirection errors
 * @param file The filename that is ambiguous
 */
void	ambiguous_redirect_error(char *file)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ambiguous redirect\n", 2);
	g_exit_status = 1;
}

/**
 * Handles syntax errors with line information
 * @param msg Error message
 * @param token The token causing the error
 * @param line_num Line number where error occurred
 */
void	enhanced_syntax_error(char *msg, char *token, int line_num)
{
	ft_putstr_fd("minishell: ", 2);
	if (line_num > 0)
		ft_putstr_fd("line ", 2);
		// TODO: Add line number conversion to string
	ft_putstr_fd("syntax error", 2);
	if (msg)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(msg, 2);
	}
	if (token)
	{
		ft_putstr_fd(" near token `", 2);
		ft_putstr_fd(token, 2);
		ft_putstr_fd("'", 2);
	}
	ft_putstr_fd("\n", 2);
	g_exit_status = 2;
}
