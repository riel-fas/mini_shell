/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_run.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:44:04 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:44:11 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

bool	check_is_directory(char *path)
{
	struct stat	sb;

	if (path == NULL || *path == '\0' || ft_strchr(path, '/') == NULL)
		return (false);
	if (stat(path, &sb) == -1)
		return (false);
	return (S_ISDIR(sb.st_mode));
}

void	handle_execution(t_cmd *current_cmd, t_list *env)
{
	char	*cmd_path;
	char	**envp;
	char	*cmd_name;
	int		no_file;

	envp = NULL;
	cmd_name = NULL;
	no_file = 0;
	if (current_cmd == NULL || current_cmd->cmd[0] == NULL)
		exit(0);
	if (current_cmd->cmd[0])
		cmd_name = ft_strdup(current_cmd->cmd[0]);
	cmd_path = check_exec(current_cmd->cmd[0], env, &no_file);
	if (cmd_path == NULL || !cmd_name[0])
	{
		free(cmd_name);
		printing_exit(current_cmd, no_file);
	}
	if (check_is_directory(cmd_path))
		free_dir(&cmd_path, &cmd_name, current_cmd);
	envp = get_env(env);
	if (envp == NULL)
		handle_exec_error(cmd_path, cmd_name);
	if (execve(cmd_path, current_cmd->cmd, envp) == -1)
		free_error_handling(&cmd_path, &cmd_name, envp);
}

void	check_is_builtin(t_cmd *current_cmd, t_list **env, t_shell *shell)
{
	char	**cmd;

	if (!current_cmd || !current_cmd->cmd)
		return ;
	cmd = current_cmd->cmd;
	if (cmd && cmd[0] && is_builtin(cmd[0]) == 0)
	{
		if (run(current_cmd, env, shell))
			exit(shell->exit_status);
	}
}

void	handle_redirections(t_cmd *current_cmd)
{
	t_cmd	*tmp;
	t_redi	*redi_list;

	tmp = current_cmd;
	redi_list = tmp->redirections;
	while (redi_list)
	{
		if (redi_list->type == INPUT || redi_list->type == HEREDOC)
			handle_input(redi_list);
		if (redi_list->type == OUTPUT || redi_list->type == APPEND)
			handle_output(redi_list);
		redi_list = redi_list->next;
	}
	tmp = tmp->next;
}

void	ft_child_ctx(t_child_ctx *ctx)
{
	setup_child_signals();
	if (ctx->tmp_in != 0 && dup2(ctx->tmp_in, STDIN_FILENO) == -1)
		command_error(NULL, "dup2 failure", 1);
	if (ctx->current_cmd->next && dup2(ctx->p_fd[1], STDOUT_FILENO) == -1)
		command_error(NULL, "dup2 failure", 1);
	if (ctx->tmp_in != 0)
		close(ctx->tmp_in);
	if (ctx->current_cmd->next)
	{
		close(ctx->p_fd[1]);
		close(ctx->p_fd[0]);
	}
	handle_redirections(ctx->current_cmd);
	check_is_builtin(ctx->current_cmd, &ctx->env, ctx->shell);
	handle_execution(ctx->current_cmd, ctx->env);
}
