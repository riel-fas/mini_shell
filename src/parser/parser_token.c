/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 20:04:54 by roubelka          #+#    #+#             */
/*   Updated: 2025/07/03 02:55:50 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"
#include "../../includes/lexer.h"

static char	*process_heredoc_delimiter(char *raw_delimiter);

t_cmds  *init_new_command(t_cmds **head, t_cmds **tail)
{
    t_cmds  *cmd;
    cmd = malloc(sizeof(t_cmds));// malloc command sie dyala
    if (!cmd)
        return (NULL);
    cmd->args = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->rw_file = NULL;
	cmd->append_node = 0;
	cmd->heredoc_delimeter = NULL;
	cmd->heredoc_list = NULL;
	cmd->heredoc_fd = -1;
	cmd->next = NULL;

	if (!*head) // if list khawya khadi iwli cmd howa head
		*head = cmd;
	else
		(*tail)->next = cmd; //adaptit tail bach iwli pointer ljdid
	*tail = cmd;
    return (cmd);
}

void	add_arg_to_cmd(t_cmds *cmd, char *value)
{
	int count;
		count = 0; //3adad args likaynin fargs
	while (cmd->args && cmd->args[count])
		count++;
	char **new_args = realloc(cmd->args, sizeof(char *) * (count + 2)); // nzido size dyal args jdida whay(+ 2) 1 dyal args jdid 2 dyal NULL
	if (!new_args)
		return ;
	cmd->args = new_args; // kanhadto cmd->args bjdid lizdna fnew->args
	cmd->args[count] = strdup(value);
	cmd->args[count + 1] = NULL;
}

static void	add_split_args_to_cmd(t_cmds *cmd, char *value)
{
	char	**words;
	int		i;
	int		word_count;
	int		start;
	int		j;
	char	*word;

	if (!cmd || !value || !*value)
		return;

	// Count words
	word_count = 0;
	i = 0;
	while (value[i])
	{
		while (value[i] && (value[i] == ' ' || value[i] == '\t'))
			i++;
		if (value[i])
		{
			word_count++;
			while (value[i] && value[i] != ' ' && value[i] != '\t')
				i++;
		}
	}

	if (word_count == 0)
		return;

	words = malloc(sizeof(char *) * (word_count + 1));
	if (!words)
		return;

	// Extract words
	word_count = 0;
	i = 0;
	while (value[i])
	{
		while (value[i] && (value[i] == ' ' || value[i] == '\t'))
			i++;
		if (value[i])
		{
			start = i;
			while (value[i] && value[i] != ' ' && value[i] != '\t')
				i++;
			word = malloc(i - start + 1);
			if (word)
			{
				for (j = 0; j < i - start; j++)
					word[j] = value[start + j];
				word[j] = '\0';
				words[word_count++] = word;
			}
		}
	}
	words[word_count] = NULL;

	// Add each word as a separate argument
	for (i = 0; words[i]; i++)
	{
		add_arg_to_cmd(cmd, words[i]);
		free(words[i]);
	}
	free(words);
}

static int	contains_unquoted_variables(char *str)
{
	int	i = 0;
	int	in_quotes = 0;
	char quote_char = 0;

	if (!str)
		return (0);

	while (str[i])
	{
		if (!in_quotes && (str[i] == '"' || str[i] == '\''))
		{
			in_quotes = 1;
			quote_char = str[i];
		}
		else if (in_quotes && str[i] == quote_char)
		{
			in_quotes = 0;
		}
		else if (!in_quotes && str[i] == '$' && str[i + 1] &&
				 (ft_isalpha(str[i + 1]) || str[i + 1] == '_' ||
				  str[i + 1] == '?' || str[i + 1] == '$' ||
				  ft_isdigit(str[i + 1])))
		{
			return (1);
		}
		i++;
	}
	return (0);
}

static int	should_word_split(char *original_value, char *expanded_value)
{
	int	i = 0;

	// Safety checks
	if (!original_value || !expanded_value)
		return (0);

	// Only split if original contained unquoted variables
	if (!contains_unquoted_variables(original_value))
		return (0);

	// And the expanded result contains whitespace
	while (expanded_value[i])
	{
		if (expanded_value[i] == ' ' || expanded_value[i] == '\t')
			return (1);
		i++;
	}
	return (0);
}
void handle_redirections(t_cmds *cmd, t_token **tokens)
{
	t_token *current = *tokens;

	if (!current || !current->next)
		return;
	if (current->type == TOKEN_REDIR_IN)
		cmd->input_file = ft_strdup(current->next->value);
	else if (current->type == TOKEN_REDIR_OUT) {
		cmd->output_file = ft_strdup(current->next->value);
		cmd->append_node = 0;
	}
	else if (current->type == TOKEN_REDIR_APPEND) {
		cmd->output_file = ft_strdup(current->next->value);
		cmd->append_node = 1;
	}
	else if (current->type == TOKEN_REDIR_READ_WRITE) {
		cmd->rw_file = ft_strdup(current->next->value);
	}
	else if (current->type == TOKEN_HEREDOC) {
		// Process heredoc delimiter: use expanded value for variable expansion,
		// but still process quotes
		char *raw_delimiter = current->next->value;
		char *processed_delimiter = process_heredoc_delimiter(raw_delimiter);

		if (!processed_delimiter)
			processed_delimiter = ft_strdup(raw_delimiter);

		// Add to heredoc list
		t_heredoc_list *new_heredoc = malloc(sizeof(t_heredoc_list));
		if (new_heredoc) {
			new_heredoc->delimiter = ft_strdup(processed_delimiter);
			new_heredoc->next = NULL;

			// Add to end of list
			if (!cmd->heredoc_list) {
				cmd->heredoc_list = new_heredoc;
			} else {
				t_heredoc_list *temp = cmd->heredoc_list;
				while (temp->next)
					temp = temp->next;
				temp->next = new_heredoc;
			}
		}

		// Keep track of the last delimiter for compatibility
		if (cmd->heredoc_delimeter) {
			free(cmd->heredoc_delimeter);
		}
		cmd->heredoc_delimeter = ft_strdup(processed_delimiter);
		free(processed_delimiter);
	}
	*tokens = current->next->next; // skip both the redirection operator and the filename
}

static char	*process_heredoc_delimiter(char *raw_delimiter)
{
	char	*result;
	int		i;
	int		j;
	int		len;
	char	quote;

	if (!raw_delimiter)
		return (NULL);

	len = ft_strlen(raw_delimiter);
	result = malloc(len + 1);
	if (!result)
		return (NULL);

	i = 0;
	j = 0;
	while (raw_delimiter[i])
	{
		if (raw_delimiter[i] == '\'' || raw_delimiter[i] == '"')
		{
			quote = raw_delimiter[i];
			i++; // Skip opening quote
			// Copy everything until closing quote, but don't include quotes
			while (raw_delimiter[i] && raw_delimiter[i] != quote)
			{
				result[j++] = raw_delimiter[i++];
			}
			if (raw_delimiter[i] == quote)
				i++; // Skip closing quote
		}
		else if (raw_delimiter[i] == '$' && raw_delimiter[i + 1] &&
				 (raw_delimiter[i + 1] == '"' || raw_delimiter[i + 1] == '\''))
		{
			// Handle $"..." and $'...' - skip the $ and process the quoted string
			i++; // Skip the $
			quote = raw_delimiter[i];
			i++; // Skip opening quote
			// Copy everything until closing quote, but don't include quotes
			while (raw_delimiter[i] && raw_delimiter[i] != quote)
			{
				result[j++] = raw_delimiter[i++];
			}
			if (raw_delimiter[i] == quote)
				i++; // Skip closing quote
		}
		else
		{
			// Copy regular characters (including $ if not followed by quotes)
			result[j++] = raw_delimiter[i++];
		}
	}
	result[j] = '\0';
	return (result);
}

t_cmds	*parse_tokens(t_token *tokens)
{
	t_cmds	*head = NULL;
	t_cmds	*tail = NULL;
	t_cmds	*current_cmd = NULL;

	while (tokens)
	{
		if (tokens->type == TOKEN_WORD || tokens->type == TOKEN_ARGUMENT)
		{
			if (!current_cmd)
				current_cmd = init_new_command(&head, &tail);

			// Check if this token should be word-split (unquoted variable expansion)
			if (should_word_split(tokens->original_value, tokens->value))
				add_split_args_to_cmd(current_cmd, tokens->value);
			else
				add_arg_to_cmd(current_cmd, tokens->value);
		}
		else if (tokens->type == TOKEN_SINGLE_QUOTED || tokens->type == TOKEN_DOUBLE_QUOTED)
		{
			if (!current_cmd)
				current_cmd = init_new_command(&head, &tail);
			// Quoted tokens are never word-split
			add_arg_to_cmd(current_cmd, tokens->value);
		}
		else if (tokens->type == TOKEN_PIPE || tokens->type == TOKEN_SEMICOLON)
		{
			current_cmd = NULL;
		}
		else if (tokens->type == TOKEN_REDIR_IN ||
				 tokens->type == TOKEN_REDIR_OUT ||
				 tokens->type == TOKEN_REDIR_APPEND ||
				 tokens->type == TOKEN_REDIR_READ_WRITE ||
				 tokens->type == TOKEN_HEREDOC)
		{
			if (!current_cmd)
				current_cmd = init_new_command(&head, &tail);
			handle_redirections(current_cmd, &tokens);
			continue;
		}
		tokens = tokens->next;
	}
	return (head);
}
