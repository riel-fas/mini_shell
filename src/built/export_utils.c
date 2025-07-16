/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riel-fas <riel-fas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:33:22 by riel-fas          #+#    #+#             */
/*   Updated: 2025/07/16 00:33:44 by riel-fas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mini_shell.h"

void	env_writen(t_list *head)
{
	while (head)
	{
		if (head && head->key)
		{
			if (head->value == NULL)
				printf("declare -x %s\n", head->key);
			else
			{
				printf("declare -x %s=", head->key);
				if (head->value[0] == '\0')
					printf("\"\"\n");
				else
					printf("\"%s\"\n", head->value);
			}
		}
		head = head->next;
	}
}

void	sort_env(t_list **env)
{
	t_list	*ptr1;
	t_list	*head;
	t_list	*temp;
	int		size_list;

	if (*env == NULL)
		return ;
	ptr1 = copy_list(*env);
	head = ptr1;
	size_list = ft_lstsize(ptr1);
	while (size_list > 0)
	{
		while (ptr1 && ptr1->next)
		{
			list_swaping(ptr1);
			ptr1 = ptr1->next;
		}
		ptr1 = head;
		size_list--;
	}
	temp = head;
	env_writen(temp);
	free_list(&head);
}

void	list_swaping(t_list *ptr1)
{
	char	*tmp_key;
	char	*tmp_value;

	tmp_key = NULL;
	tmp_value = NULL;
	if (ft_strcmp(ptr1->key, ptr1->next->key) > 0)
	{
		tmp_key = ptr1->key;
		tmp_value = ptr1->value;
		ptr1->key = ptr1->next->key;
		ptr1->value = ptr1->next->value;
		ptr1->next->key = tmp_key;
		ptr1->next->value = tmp_value;
	}
}

void	alloc_2(t_list *dup_key, char **key, char **value, char *cmd)
{
	if (!dup_key || !key || !value)
		return ;
	if (ft_strchr(cmd, '='))
	{
		if (dup_key->value)
			free(dup_key->value);
		dup_key->value = *value;
		*value = NULL;
	}
	else if (*value)
	{
		free(*value);
		*value = NULL;
	}
	if (*key)
	{
		free(*key);
		*key = NULL;
	}
}
