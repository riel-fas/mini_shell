/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 11:16:46 by riel-fas          #+#    #+#             */
/*   Updated: 2025/05/17 13:00:13 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
//storina env f struct dyal shell **env
//jbedna username men env o storinah *username
//jbedna men env PATHS bach khadi nehtajohom f execution
//kandiro copy l env fwest l minishell dyalna
char	**copy_env(char **env)
{
	int		x;
	int		y;
	char	**env_copy;

	x = 0;
	while (env[x]) // we count the elements in the env
		x++;
	env_copy = malloc(sizeof(char *) * (x + 1));
	if (!env_copy)  // UNCOMMENTED: Important error check
		return (NULL);
	y = 0;
	while (y < x)
	{
		env_copy[y] = ft_strdup(env[y]);
		y++;
	}
	env_copy[y] = NULL;  // FIXED: Use NULL not '\0'
	return (env_copy);
}

//kan splitiw l path 3la 7ssab execution
char	**split_paths(char **env)  // FIXED: Return type changed to char**
{
	char	**paths;

	while (*env && ft_strncmp(*env, "PATH=", 5) != 0)
		env++;
	if (!*env)
		return (NULL);
	paths = ft_split(*env + 5, ':');
	return (paths);
}

//kanjebdo username men env
char	*get_username(char **env)
{
	int	i;

	i = 0;  // FIXED: Initialized i
    // First try USER environment variable
	while (env[i])
	{
		if (ft_strncmp(env[i], "USER=", 5) == 0)
			return ft_strdup(env[i] + 5);
		i++;
	}
    // Default fallback
	return (ft_strdup("user"));
}

t_shell	*shell_init(char **env)
{
	t_shell *shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)  // UNCOMMENTED: Important error check
		return (NULL);
	//function to copy the env into a double pointer
	shell->env = copy_env(env);
    //extract the username from the env
	shell->username = get_username(env);
    //split PATH in env by ":" for CMDS execution
	shell->path = split_paths(env);
    return (shell);
}
