/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 12:19:00 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 02:40:17 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"
#include "../../includes/executor.h"
#include "../../includes/error.h"

void	enhanced_exec_error(char *cmd_path, char *cmd_name)
{
	if (!cmd_path || !cmd_name)
		return ;
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
		ft_cmd_error(cmd_name, strerror(errno), 126);
}

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
	g_state.exit_status = err;
}

void	ambiguous_redirect_error(char *file)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ambiguous redirect\n", 2);
	g_state.exit_status = 1;
}

void	enhanced_syntax_error(char *msg, char *token, int line_num)
{
	ft_putstr_fd("minishell: ", 2);
	if (line_num > 0)
		ft_putstr_fd("line ", 2);
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
	g_state.exit_status = 2;
}
