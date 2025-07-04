/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:45:00 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/04 21:31:58 by codespace        ###   ########.fr       */
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

//child_exec.c
void				execute_child_command(t_shell *shell, t_cmds *cmd);

//builtin_resolver.c
t_builtin_func		get_builtin(char *cmd_name);
int					is_builtin(char *cmd_name);

//cmd_path.c
char				*get_command_path(char *cmd, t_env *env);

//cmd_utils.c
int					check_command_access(char *path);
void				handle_execution_error(char *cmd_path, char *cmd_name);
void				ft_cmd_error(char *cmd, char *error, int status);
void				free_env_array(char **env_array);

//env_converter.c
char				**convert_env_to_array(t_env *env);

//error_check.c
int					check_redirection_errors(t_cmds *cmd);
int					check_pipeline_errors(t_cmds *commands, t_env *env);

//error_detection.c
int					check_command_errors(char *cmd, t_env *env);
int					detect_execution_errors(t_shell *shell, t_cmds *commands);

//error_handling.h
void				enhanced_exec_error(char *cmd_path, char *cmd_name);
void				enhanced_redir_error(char *file, char *msg, int err);
void				ambiguous_redirect_error(char *file);
void				enhanced_syntax_error(char *msg, char *token, int line_num);

//exec_signals.c
void				handle_child_sigint(int sig);
void				handle_child_sigquit(int sig);
void				setup_child_signals(void);
void				handle_parent_sigint(int sig);
void				setup_parent_waiting_signals(void);

//exec_utils.c
char				*find_command_path(char **paths, char *cmd);
void				error_message(char *cmd, char *msg);

//execution_cleanup.c
void				finish_execution(pid_t last_pid);

//executor_process.c
void				ft_child_process(t_cmds *cmd, t_env *env,
						int input_fd, int *pipe_fds);

//executor.c
int					execute_commands(t_shell *shell, t_cmds *commands);

//file_operations.c
int					open_input_file(char *file);
int					open_output_file(char *file, int append);
void				reset_redirections(int stdin_backup, int stdout_backup);
int					handle_output_redirection(t_cmds *cmd);
int					handle_input_redirection(t_cmds *cmd);

//heredoc_exec.c
int					process_command_heredocs(t_cmds *current);
int					process_heredoc_for_pipeline(t_cmds *cmd, int *heredoc_fd);

//heredoc_list.c
int					process_heredoc_list(t_cmds *cmd);
int					process_single_heredoc(t_heredoc_list *current,
						t_cmds *cmd, int is_last);
int					process_all_heredocs(t_cmds *cmd);

//heredoc_main.c
int					process_heredocs_after_parsing(t_cmds *commands);

//heredoc_pipe.c
void				process_heredoc_child(int write_fd, char *delimiter);
int					handle_heredoc_parent(pid_t pid, int read_fd);
int					create_heredoc_pipe(t_cmds *cmd, int *heredoc_fd);

//heredoc_signals.c
void				handle_parent_heredoc_sigint(int sig);
void				handle_heredoc_sigint(int sig);
void				setup_heredoc_signals(void);
void				setup_parent_heredoc_signals(void);
void				restore_heredoc_signals(void);

//pipeline_exec.c
int					execution(t_cmds *commands, t_env *env);
void				ft_parent_process(int *input_fd, int *pipe_fds,
						t_cmds *cmd);
int					create_pipe_safe(int *pipe_fds);

//redirection.c
int					handle_heredoc_redirection(t_cmds *cmd);
int					handle_rw_redirection(t_cmds *cmd);
int					handle_exec_redirections(t_cmds *cmd);
int					setup_redirections(t_cmds *cmd);

//signal_restoration.c
void				restore_signals(void);

//single_exec_utils.c
int					execute_builtin_with_redirections(t_shell *shell,
						t_cmds *cmd);
int					execute_external_command(t_shell *shell, t_cmds *cmd);

//single_exec.c
int					execute_single_command_advanced(t_shell *shell,
						t_cmds *cmd);
int					handle_empty_command_redirections(t_cmds *cmd);
int					execute_single_command(t_shell *shell, t_cmds *cmd);

#endif
