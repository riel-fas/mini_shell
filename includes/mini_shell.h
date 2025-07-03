/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:19:59 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/03 01:10:22 by codespace        ###   ########.fr       */
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
#include <sys/types.h>
# include <errno.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
// # include "lexer.h"


# define READ_END 0
# define WRITE_END 1


/*enum _____ types for lexer\*/
typedef enum e_token_type
{
	TOKEN_WORD, //cmds ls or rm
	TOKEN_ARGUMENT, // -l or -rf
	TOKEN_PIPE, //|
	TOKEN_REDIR_IN, //<
	TOKEN_REDIR_OUT, //>
	TOKEN_REDIR_APPEND, //<<
	TOKEN_HEREDOC, //>>
	TOKEN_REDIR_READ_WRITE, //<> - open file for both reading and writing
	TOKEN_END_OF_INPUT, //end of input (heredoc)
	TOKEN_SINGLE_QUOTED, // 'text' - no expansion
	TOKEN_DOUBLE_QUOTED, // "text" - allow expansion
	TOKEN_SEMICOLON, // ; - command separator
}	t_token_type;

//token struct
typedef struct s_token
{
	char			*value;
	char			*original_value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

//struct for commands
typedef struct  s_cmds
{
	char			**args; //cmd with the argument
	char			*input_file; //input for redir. file
	char			*output_file; //output for redir. file
	char			*rw_file; //read-write redirection file (<>)
	int				append_node;	//?
	char			*heredoc_delimeter; //heredoc delimeter
	struct s_cmds	*next; //next cmd in the pipeline |
}	t_cmds;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

//mini_shell Data_Structure
typedef struct s_shell
{
	t_env			*env; //ENVIRONMENT // AR: env li khadi ndirolo copy 3endna f lprojet
	char			*username; //USR ENV \\AR: USERNAME li katel9ah f path USR f west l ENV
	char			**path; //stores the paths in env   // AR: fiha dok path men ba3d ma splitinahom  ":"
	t_token			*tokens; //INPUT AFTER IT HAS BEEN TOKENISED
	t_cmds			*commands; //COMMANDS AFTER PARSING
	int				exit_status; //EXIT STATUS OF LAST COMMAND
}	t_shell;

typedef enum e_parse_result
{
	PARSE_SUCCESS,
	PARSE_ERROR_SYNTAX,
	PARSE_ERROR_MEMORY
}	t_parse_result;

typedef int	(*t_builtin_func)(t_shell *shell, char **args);

int				execute_commands(t_shell *shell, t_cmds *commands);
t_cmds		*parse_tokens(t_token *tokens);
void			print_commands(t_cmds *cmds);
void			free_commands(t_cmds *cmds);





//mini_shell_utils.c
int ft_strcmp(char *s1, char *s2);

//copy_env.c
t_env    *create_env_list(char **env);
char    *get_username(t_env *env);
char    **split_paths(t_env *env);
void     update_shell_path(t_shell *shell);

//env_list.c
t_env   *new_env_node(char *name, char *value);
void    add_env_node(t_env **env_list, t_env *new_node);
t_env   *find_env_var(t_env *env, char *key);
char    *get_env_value(t_env *env, char *key);
char    **env_list_to_array(t_env *env);

//signal.c
void    handler(int sig);
void sigquit_handler(int signum);

//heredoc_signal.c
void	setup_heredoc_signals(void);
void	handle_heredoc_sigint(int sig);
void	setup_parent_heredoc_signals(void);
void	handle_parent_heredoc_sigint(int sig);
void	restore_heredoc_signals(void);

//clean_mini_shell.c
void    cleanup(t_shell *shell);

t_shell    *shell_init(char **env);
int	minishell_loop(t_shell *shell);

//lexer_utils.c
int		is_whitespace(char c);
int		is_operator(char c);
int		is_quote(char c);
t_token	*create_token(char *value, t_token_type type);
void	add_token(t_token **tokens, t_token *new_token);
void	free_tokens(t_token *tokens);

//lexer_extraction.c
char	*extract_word(char *input, int *i);
char	*extract_operator(char *input, int *i, t_token_type *type);
char	*extract_quoted_string(char *input, int *i, char quote);

//lexer.c
t_token	*tokenize(char *input);
void	print_tokens(t_token *tokens);

//mini_shell_input.c
int		process_exit_check(char *input);
int		handle_tokenize_error(t_shell *shell, char *input);
int		handle_parse_error(t_shell *shell, t_parse_result result);
int		is_only_whitespace(char *input);

#endif
