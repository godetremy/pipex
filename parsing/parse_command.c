/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgodet <rgodet@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 12:11:42 by rgodet            #+#    #+#             */
/*   Updated: 2025/01/15 10:12:11 by rgodet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static char	*join_path(char *path, char *cmd)
{
	char	*path_with_slash;
	char	*path_with_cmd;

	path_with_slash = ft_strjoin(path, "/");
	path_with_cmd = ft_strjoin(path_with_slash, cmd);
	free(path_with_slash);
	return (path_with_cmd);
}

static void	free_path_env(char **path)
{
	int	i;

	i = 0;
	while (path[i])
	{
		free(path[i]);
		i++;
	}
	free(path);
}

static char	*get_command_path(char *cmd, char **envp)
{
	char	**path;
	int		i;
	int		file_access;
	char	*cmd_path;

	path = get_path_env(envp);
	if (!path)
		return (NULL);
	i = 0;
	while (path[i])
	{
		cmd_path = join_path(path[i], cmd);
		file_access = access(cmd_path, F_OK | X_OK);
		if (file_access == 0)
		{
			free_path_env(path);
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	free_path_env(path);
	return (NULL);
}

static char	*get_dot_slash_path(char *cmd, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PWD=", 4) == 0)
		{
			return (ft_strjoin(envp[i] + 4, cmd));
		}
		i++;
	}
	return (NULL);
}

t_cmd	parse_command(char *cmd, char **envp)
{
	t_cmd	command;

	command.args = ft_split(cmd, ' ');
	if (*cmd == '\0')
		command.path = NULL;
	else if (*cmd == '/')
		command.path = ft_strdup(cmd);
	else if (*cmd == '.' && *(cmd + 1) == '/')
		command.path = get_dot_slash_path(cmd + 1, envp);
	else
		command.path = get_command_path(command.args[0], envp);
	if (command.path == NULL)
    {
        free(command.args);
        command.args = NULL;
    }
	return (command);
}
