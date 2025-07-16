/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 01:44:17 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 01:45:11 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_SHELL_H
# define MINI_SHELL_H

# include "../libft/libft.h"
# include "get_next_line.h"
# include <curses.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_shell
{
	int	exit_status;
}						t_shell;

typedef enum e_token_type
{
	WORD,
	PIPE,
	INPUT,
	OUTPUT,
	HEREDOC,
	APPEND,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
}						t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	bool				addspace;
	bool				variable;
	struct s_token		*current;
	struct s_token		*next;
	struct s_token		*prev;
}						t_token;

typedef struct s_lexer
{
	char				*input;
	int					pos;
	int					len;
}						t_lexer;

typedef struct s_redi
{
	t_token_type		type;
	char				*file;
	char				*content;
	bool				is_last;
	int					tmp_fd;
	bool				is_ambiguous;
	bool				expand;
	bool				variable;
	int					heredoc_fd;
	struct s_redi		*next;
}						t_redi;

typedef struct s_cmd
{
	char				**cmd;
	int					cmdsize;
	int					cmd_capacity;
	bool				is_builtin;
	int					origin_stdout;
	int					origin_stdin;
	t_redi				*redirections;
	t_redi				*input;
	t_redi				*output;
	struct s_cmd		*next;
}						t_cmd;

typedef struct s_heredoc_ctx
{
	char				*delimiter;
	int					*fd_pipe;
	t_redi				*heredoc;
	t_list				*env;
	t_shell				*shell;
}						t_heredoc_ctx;

typedef struct s_parent_ctx
{
	int					*fd_pipe;
	int					pid;
	int					*status;
	t_redi				*in;
	t_shell				*shell;
}						t_parent_ctx;

typedef struct s_child_ctx
{
	t_cmd				*current_cmd;
	t_list				*env;
	int					tmp_in;
	int					*p_fd;
	t_shell				*shell;
}						t_child_ctx;

typedef struct s_x
{
	t_lexer				**lexer;
	t_list				*minienv;
	bool				*heredoc;
	t_shell				*shell;
}						t_x;

typedef struct s_next_token_ctx
{
	t_lexer				*lexer;
	t_list				*minienv;
	bool				*heredoc;
	t_shell				*shell;
}						t_next_token_ctx;

typedef struct s_exec_ctx
{
	t_cmd				*current_cmd;
	t_list				*env;
	t_shell				*shell;
	int					*tmp_in;
	int					*pip_fd;
	pid_t				*last_cmd_pid;
}						t_exec_ctx;

# define HEREDOC_FILE "/tmp/minishell_heredoc_tmp"
# define UNCLOSED "unexpected EOF while looking for matching quote\n"
# define EXIT_ERROR "exit: numeric argument required"
# define PIPE_ERROR "syntax error near unexpected token `|'\n"
# define SYNTAX_ERROR "syntax error near unexpected token `newline'\n"
# define CD_HOME_ERR "minishell :cd: HOME not set\n"
# define CD_OLDPWD_ERR "minishell :cd: OLDPWD not set\n"

/*------------UTILS------------*/
int		skip_char(char *input, int i);
bool	quotes_scan(char *input, t_shell *shell);
t_cmd	*create_new_node(t_cmd *node);
void	add_node_back(t_cmd **lst, t_cmd *new);
t_cmd	*node_init(void);
t_cmd	*next_node(t_token *token_list, t_shell *shell);
bool	cmd_array_res(t_cmd **node, int new_capacity);
void	minishell(char *input, t_list **minienv);
int		ft_isspace(char c);
int		is_red_pip(char c);
bool	check_duplicated(t_lexer *lexer, char op[3]);
ssize_t	check_dollar(char *str);
int		var_size(char *str);
void	clean_mini(t_token *token_list, t_cmd *cmdarg_list);
int		syntax_check(t_token *token_list, t_shell *shell);
bool	redirection_errors(t_token *current);
void	pipe_closed(t_redi *redic);
bool	pipe_errors(t_token *current);
void	free_cmd(t_cmd *cmdarg_list);
void	free_token_list(t_token *token_list);
void	free_token(t_token *token);
void	free_node(t_cmd *node);
void	free_redirection(t_redi *redi);

/*----------------SIGNAL-----------*/
void	set_signal_shell(t_shell *shell);
void	handle_sigint(int sig);
void	handle_parent_sigint(int sig);
void	handle_parent_heredoc_sigint(int sig);
void	setup_heredoc_signals(void);
void	restore_signals(void);
void	setup_child_signals(void);
void	setup_parent_waiting_signals(void);
void	setup_parent_heredoc_signals(void);
void	handle_heredoc_sigint(int sig);
void	handle_signals(void);

/*-------------REDIRECTIONS-----------*/
bool	rediction_rec(t_token *token);
t_redi	*set_new_redir(t_token *token, bool expand);
void	add_redir(t_redi **redi, t_redi *new);

/*-------------PARSER----------------*/
void	*word_parsing(t_cmd **node, t_token *token_list, t_shell *shell);
void	redi_pars(t_cmd **node, t_token *token_list);
t_cmd	*parsing(t_token *token_list, t_list *minienv, t_shell *shell);
bool	dollar_exp(t_token **token, ssize_t dollar_pos);
char	*expanding(t_token **token, t_list *minienv, ssize_t dollar_pos);
char	*remaining_por(t_token **token, ssize_t dollar_pos);
char	*str_join_free(char *s1, char *s2);
int		count(char *s, char p);
void	skip(char **s, int *len, char p, char **start);
void	edit(int len, int j, char *start, char **buff);
char	**parsing_split(char *s, char p);
void	expanding_exit(t_token **token, t_shell *shell);
void	expanding_norm(t_token **token, t_list *minienv, ssize_t dpos);
void	variables_expanding(t_token **token, t_list *minienv, t_shell *shell);
char	*env_getting(char *var, t_list *minienv);
char	*var_getting(char *var, t_list *minienv);
char	**freeall(char **p, size_t x);

/*---------------LEXER----------------------*/
void	add_toekn_back(t_token **token_list, t_token *token);
void	expanding_var_char(char **value, t_list *minienv, t_shell *shell);
bool	check_if_cmd(t_token *current);
void	extend_cmd(t_cmd **node, int needed_capacity, t_shell *shell);
void	add_first_word(t_cmd **node, char **split_words, bool first_word_joins);
void	add_splited_words(t_cmd **node,
			char **split_words, bool first_word_joins, t_shell *shell);
void	free_splited_words(char **split_words);
void	splited_words_handling(t_cmd **node,
			t_token *token_list, char *val, t_shell *shell);
void	token_init_arg(t_token *token);
t_token	*handle_tok_quotes(t_lexer *lexer, char quote_char, t_shell *shell);
void	token_type_set(t_token *token, int op_len, char *op, bool *heredoc);
t_token	*ft_handle_operator(t_lexer *lexer, bool *heredoc);
t_token	*word_handling(t_lexer *lexer);
t_token	*ft_get_next_token_ctx(t_next_token_ctx *ctx);
t_lexer	*lexer_init(char *input);
t_token	*is_string_token(char *input, t_list *minienv, t_shell *shell);
bool	to_join(t_lexer *lexer);
t_token	*new_token_set(t_token *token);
t_token	*dollar_sign_joining(t_token *token, t_token *new_token);
t_token	*ft_jointok_ctx_finish(t_token *token, t_token *n);
t_token	*ft_jointok_ctx(t_token *token, t_x *ctx);
t_token	*ft_get_next_token_ctx(t_next_token_ctx *ctx);
int		toksizer(t_token *lst);

/*--------------HEREDOC---------------*/
int		handel_heredoc(t_redi *redi, t_list *env, t_shell *shell);
int		here_doc_check(t_cmd *shell, t_list *env, t_shell *sh);
void	init_redi_file(t_cmd *shell);
int		handle_heredoc_break(char *line, char *delimiter, t_shell *shell);
void	read_line_by_line(t_heredoc_ctx *ctx);
int		open_here_doc_ctx(t_heredoc_ctx *ctx);
void	parent_ctx(t_parent_ctx *ctx);
bool	dollar_char(char *value, ssize_t dollar_pos);
char	*expanding_char(char *value, t_list *minienv, ssize_t dollar_pos);
void	expanding_exit_char(char **value, t_shell *shell);
char	*remaining_char_tmp(char *value, ssize_t dollar_pos);

/*----------------EXEC----------------*/
void	wait_for_children(int *status, pid_t last_cmd_pid, t_shell *shell);
int		open_file(char *file, int flag);
void	command_error(char *cmd_name, char *error, int status);
void	handle_output(t_redi *output);
int		ft_strcmp(const char *s1, const char *s2);
void	handle_input(t_redi *input);
char	*check_full_path(char *p);
void	handle_exec_error(char *cmd_path, char *cmd_name);
bool	check_is_directory(char *path);
void	handle_execution(t_cmd *current_cmd, t_list *env);
void	check_is_builtin(t_cmd *current_cmd, t_list **env, t_shell *shell);
void	handle_redirections(t_cmd *current_cmd);
void	ft_child_ctx(t_child_ctx *ctx);
void	printing_exit(t_cmd *current_cmd, int no_file);
void	handle_heredoc(t_redi *input);
int		handel_append(t_redi *output);
void	free_list(t_list **list);
char	*find_path(t_list *path);
void	get_the_path(char **full_path, char **path_cmd, char *p);
char	*joining_path(char *p);
char	*check_exec(char *p, t_list *env, int *no_file);
bool	ambiguous_error(char *file);
void	finish_exec(pid_t last_cmd_pid, t_shell *shell);
int		size_list(t_cmd *node);
void	free_all(char **bf, int j);
char	**get_env(t_list *env);
void	free_dir(char **cmd_path, char **cmd_name, t_cmd *current_cmd);
void	free_error_handling(char **cmd_path, char **cmd_name, char **envp);
void	parent_processing(int *tmp_in, int *pip_fd, t_cmd *current_cmd);
int		create_pipe(int *pip, t_shell *shell);
int		export_built(char **cmd, t_list **env);
void	free_every_char(char *s);
void	redi_errors(char *file, char *msg, int err, t_shell *shell);
int		open_input(t_redi *input, t_shell *shell);
int		execution(t_cmd *current_cmd, t_list *env, t_shell *shell);

/*-------------------BUILT--------------------*/
void	free_buitl(char **cmd);
int		check_builtin(t_cmd *cmdarg_list, t_list **minienv, t_shell *shell);
bool	heredoc_cant_exp(t_token *tmp);
void	std_resetting(t_cmd *shell);
int		open_redirections(t_redi *redirections, t_shell *shell);
int		is_builtin(char *cmd);
t_list	*copy_list(t_list *env);
int		run(t_cmd *cmd, t_list **env, t_shell *shell);
void	alloc_key_value(char *cmd, char **key, char **value, char *equals_pos);
int		remove_env_node(t_list **env_list, t_list *node);
int		set_env_built(t_list **env);
int		unset_built(char **cmd, t_list **env);
int		size_cd(char **c);
t_list	*node(t_list *env, char *key);
void	path_recheck(t_list *env, char *new_path, char *old_path);
int		cd_built(char **cmd, t_list **env);
int		echo_built(char **cmd, t_cmd *shell);
int		env_built(t_list **env, t_shell *shell);
int		exit_built(char **cmd, t_list **env, t_shell *shell);
void	env_writen(t_list *head);
void	sort_env(t_list **env);
void	list_swaping(t_list *ptr1);
void	alloc_2(t_list *dup_key, char **key, char **value, char *cmd);
int		print_invalid_identifier(char *cmd);
void	helper(char **key, char **value, char *cmd);
void	handle_append(t_list *dup_key, char **key, char **value);
int		handler_exp(char *cmd, char **key, char **value);
t_list	*check_dup_env(char *key, t_list *env);
void	key_value(char *cmd, char **key, char **value, t_list **env);
int		redi_for_builtin(char *file, int flag, t_shell *shell);
int		open_append(t_redi *output, t_shell *shell);
int		open_output(t_redi *output, t_shell *shell);
int		pwd_built(t_list **env);

/*------------------------------ENV---------------------------------*/
int		parse_env(char *env_var, char **key, char **value);
t_list	*init_env(char **env);

#endif
