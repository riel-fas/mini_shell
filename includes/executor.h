/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:45:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/04 03:21:39 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "mini_shell.h"
# include "builtins.h"
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdbool.h>

/* Pipe constants */
# define READ_END 0
# define WRITE_END 1

/* Global state access */
extern t_global_state	g_state;

/* Advanced redirection structure */
typedef struct s_redir_list
{
	t_token_type		type;
	char				*file;
	bool				is_last;
	int					fd;
	bool				is_ambiguous;
	struct s_redir_list	*next;
}	t_redir_list;

/* Pipeline execution context structure */
typedef struct s_pipeline_ctx
{
	int		*prev_pipe_read;
	int		cmd_index;
	int		cmd_count;
	int		heredoc_fd;
}	t_pipeline_ctx;

/* Pipeline child execution context */
typedef struct s_child_ctx
{
	int		input_fd;
	int		*pipefd;
	int		has_next;
}	t_child_ctx;

/* Pipeline parent management context */
typedef struct s_parent_ctx
{
	int		*prev_pipe_read;
	int		*pipefd;
	int		effective_input;
	int		heredoc_fd;
}	t_parent_ctx;

/* Executor functions */
int					execute_commands(t_shell *shell, t_cmds *commands);
int					execution(t_cmds *commands, t_env *env);
int					execute_command_robust(t_shell *shell, t_cmds *cmd);
int					execute_pipeline_robust(t_shell *shell, t_cmds *commands);
int					execute_commands_robust(t_shell *shell, t_cmds *commands);

/* Advanced redirection functions */
int					setup_redirections(t_cmds *cmd);
int					handle_exec_redirections(t_cmds *cmd);
int					open_input_file(char *file);
int					open_output_file(char *file, int append);
int					process_heredoc_list(t_cmds *cmd);
void				reset_redirections(int stdin_backup, int stdout_backup);
int					process_heredoc_for_pipeline(t_cmds *cmd, int *heredoc_fd);
int					create_heredoc_pipe(t_cmds *cmd, int *heredoc_fd);
int					process_heredocs_after_parsing(t_cmds *commands);

/* Advanced pipeline functions */
int					execute_single_command(t_shell *shell, t_cmds *cmd);
int					execute_pipeline(t_shell *shell, t_cmds *commands);
int					execute_pipeline_enhanced(t_shell *shell, t_cmds *commands);
int					create_pipe_safe(int *pipe_fds);
int					create_pipe_enhanced(int *pipe_fds);
int					handle_fork_failure(t_cmds *cmd);
void				safe_close(int fd);
void				ft_child_process(t_cmds *cmd, t_env *env, int input_fd,
						int *pipe_fds);
void				ft_parent_process(int *input_fd, int *pipe_fds,
						t_cmds *cmd);
void				finish_execution(pid_t last_pid);
int					verify_pipeline_commands(t_shell *shell, t_cmds *commands);
int					wait_for_all_children(pid_t *pids, int count);

/* Advanced command path functions */
char				*find_command_path(char **paths, char *cmd);
char				*get_command_path(char *cmd, t_env *env);
char				*get_command_path_enhanced(char *cmd, t_env *env);
bool				is_directory(char *path);
bool				is_full_path(char *cmd);
int					handle_path_errors(char *cmd_path, char *cmd_name);
bool				is_empty_executable(char *cmd_path);
int					check_command_access(char *path);

/* Advanced error handling */
void				error_message(char *cmd, char *msg);
void				ft_cmd_error(char *cmd, char *error, int status);
void				handle_execution_error(char *cmd_path, char *cmd_name);

/* Advanced memory management */
void				free_env_array(char **env_array);
char				**convert_env_to_array(t_env *env);

/* Signal handling for execution */
void				setup_child_signals(void);
void				setup_parent_waiting_signals(void);
void				restore_signals(void);

/* Advanced execution functions */
int					execute_single_command_advanced(t_shell *shell,
						t_cmds *cmd);
int					handle_empty_command_redirections(t_cmds *cmd);
int					execute_builtin_with_redirections(t_shell *shell,
						t_cmds *cmd);
int					execute_external_command(t_shell *shell, t_cmds *cmd);
void				execute_child_command(t_shell *shell, t_cmds *cmd);

/* Advanced pipeline functions */
int					count_pipeline_commands(t_cmds *commands);
int					prepare_pipeline_heredoc(t_cmds *commands);
pid_t				execute_pipeline_command_advanced(t_shell *shell,
						t_cmds *cmd, t_pipeline_ctx *ctx);
void				execute_pipeline_child(t_shell *shell, t_cmds *cmd,
						t_child_ctx *child_ctx);
void				handle_pipeline_parent(t_cmds *cmd,
						t_parent_ctx *parent_ctx);
int					handle_file_redirections_only(t_cmds *cmd);
int					wait_for_pipeline(pid_t *pids, int cmd_count);
void				cleanup_pipeline(pid_t *pids, int created_processes,
						int prev_pipe_read, int heredoc_fd);

/* Builtin function getter */
t_builtin_func		get_builtin(char *cmd_name);
int					is_builtin(char *cmd_name);

int	handle_output_redirection(t_cmds *cmd);
int	handle_input_redirection(t_cmds *cmd);
int	process_command_heredocs(t_cmds *current);



#endif
