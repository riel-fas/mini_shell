/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:19:59 by riel-fas          #+#    #+#             */
/*   Updated: 2025/05/19 18:51:43 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// # include "../libft/libft.h"
// # include <readline/readline.h>
// # include <readline/history.h>
// # include <stdio.h>
// # include <stdlib.h>
// # include <unistd.h>
// # include <fcntl.h>
// # include <signal.h>
// # include <sys/wait.h>


# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
// # include </Users/roubelka/.brew/opt/readline/include/readline/rlconf.h> //rachid
// # include </Users/riel-fas/.brew/opt/readline/include/readline/rlconf.h> //riad
# include "/home/linuxbrew/.linuxbrew/opt/readline/include/readline/rlconf.h" //linux riad
# include "../libft/libft.h"

# define READ_END 0
# define WRITE_END 1
// # define PROMPT "minishell$~> "

/*enum _____ types for lexer\*/
typedef enum e_token_type
{
	TOKEN_WORD, //cmds
	TOKEN_PIPE, //|
	TOKEN_REDIR_IN, //<
	TOKEN_REDIR_OUT, //>
	TOKEN_REDIR_APPEND, //>>
	TOKEN_HEREDOC, //<<
	TOKEN_END_OF_INPUT //end of input (heredoc)
}	t_token_type;

//token struct
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

//struct for commands
typedef struct  s_cmds
{
	char			**args; //cmd with the argument
	char			*input_file; //input for redir. file
	char			*output_file; //output for redir. file
	int				append_node;	//?
	char			*heredoc_delimeter; //heredoc delimeter
	struct s_cmds	*next; //next cmd in the pipeline |
}	t_cmds;

//mini_shell Data_Structure
typedef struct s_shell
{
	t_env			**env; //ENVIRONMENT // AR: env li khadi ndirolo copy 3endna f lprojet
	char			*username; //USR ENV \\AR: USERNAME li katel9ah f path USR f west l ENV
	char			**path; //stores the paths in env   // AR: fiha dok path men ba3d ma splitinahom  ":"
	t_token			*tokens; //INPUT AFTER IT HAS BEEN TOKENISED
	t_cmds			*commands; //COMMANDS AFTER PARSING
	int				exit_status; //EXIT STATUS OF LAST COMMAND
	// int				working; //SHELL IS RUNNING
}	t_shell;

typedef struct s_env
{
	char		*key;
	char		*value;
	struct s_env	*next;
}	t_env;




void	cleanup(t_shell *shell);
t_shell	*shell_init(char **env);
char	*get_username(char **env);
char	**split_paths(char **env);
char	**copy_env(char **env);

void	handler(int sig);


#endif
