/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:19:59 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/04 22:03:46 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_SHELL_H
# define MINI_SHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <errno.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include "memory.h"
# include "error.h"

# define READ_END 0
# define WRITE_END 1

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_ARGUMENT,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_REDIR_READ_WRITE,
	TOKEN_END_OF_INPUT,
	TOKEN_SINGLE_QUOTED,
	TOKEN_DOUBLE_QUOTED,
	TOKEN_SEMICOLON
}	t_token_type;

typedef struct s_token
{
	char			*value;
	char			*original_value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_heredoc_list
{
	char					*delimiter;
	struct s_heredoc_list	*next;
}	t_heredoc_list;

typedef struct s_cmds
{
	char				**args;
	char				*input_file;
	char				*output_file;
	char				*rw_file;
	int					append_node;
	char				*heredoc_delimeter;
	t_heredoc_list		*heredoc_list;
	int					heredoc_fd;
	struct s_cmds		*next;
}	t_cmds;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_env			*env;
	char			*username;
	char			**path;
	t_token			*tokens;
	t_cmds			*commands;
	int				exit_status;
}	t_shell;

typedef struct s_global_state
{
	int				exit_status;
	t_shell			*current_shell;
}	t_global_state;

extern t_global_state	g_state;

void			sync_shell_state(t_shell *shell);

typedef enum e_parse_result
{
	PARSE_SUCCESS,
	PARSE_ERROR_SYNTAX,
	PARSE_ERROR_MEMORY
}	t_parse_result;

/* Function declarations */
int				execute_commands(t_shell *shell, t_cmds *commands);
t_cmds			*parse_tokens(t_token *tokens);
void			print_commands(t_cmds *cmds);
void			free_commands(t_cmds *cmds);
int				ft_strcmp(char *s1, char *s2);
void			*ft_realloc(void *ptr, size_t old_size, size_t new_size);
t_env			*create_env_list(char **env);
char			*get_username(t_env *env);
char			**split_paths(t_env *env);
void			update_shell_path(t_shell *shell);
t_env			*new_env_node(char *name, char *value);
void			add_env_node(t_env **env_list, t_env *new_node);
t_env			*find_env_var(t_env *env, char *key);
char			*get_env_value(t_env *env, char *key);
char			**env_list_to_array(t_env *env);
void			handler(int sig);
void			sigquit_handler(int signum);
void			setup_heredoc_signals(void);
void			handle_heredoc_sigint(int sig);
void			setup_parent_heredoc_signals(void);
void			handle_parent_heredoc_sigint(int sig);
void			restore_heredoc_signals(void);
void			cleanup(t_shell *shell);
t_shell			*shell_init(char **env);
int				minishell_loop(t_shell *shell);
int				is_whitespace(char c);
int				is_operator(char c);
int				is_quote(char c);
t_token			*create_token(char *value, t_token_type type);
void			add_token(t_token **tokens, t_token *new_token);
void			free_tokens(t_token *tokens);
char			*extract_word(char *input, int *i);
char			*extract_operator(char *input, int *i, t_token_type *type);
char			*extract_quoted_string(char *input, int *i, char quote);
t_token			*tokenize(char *input);
void			print_tokens(t_token *tokens);
int				process_exit_check(char *input);
int				handle_tokenize_error(t_shell *shell, char *input);
int				handle_parse_error(t_shell *shell, t_parse_result result);
int				is_only_whitespace(char *input);
char			*get_next_line(int fd);
char			*my_strchr(char *s, char c);
char			*my_strjoin(char *s1, char *s2);
char			*my_substr(char *s, unsigned int start, size_t len);
char			*get_line(int fd, char *hold);
char			*extract_line(char **hold);
int				next_line_index(char *hold);
int				read_and_append(int fd, char *buff, char **hold);
int				process_all_heredocs(t_cmds *cmd);
int				process_heredocs_after_parsing(t_cmds *commands);
char			*generate_prompt(t_shell *shell);

#endif
