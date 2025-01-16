/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgodet <rgodet@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 09:05:29 by rgodet            #+#    #+#             */
/*   Updated: 2025/01/16 09:37:40 by rgodet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_cmd(t_cmd cmd)
{
	int	i;

	i = 0;
	while (cmd.args[i])
	{
		free(cmd.args[i]);
		i++;
	}
	free(cmd.args);
	if (cmd.path != NULL)
		free(cmd.path);
}

int	main(int argc, char **argv, char **envp)
{
	int			status;
	t_params	params;

	params = check_arguments(argc, argv);
	params.cmd1 = parse_command(params.cmd1_str, envp);
	params.cmd2 = parse_command(params.cmd2_str, envp);
	ft_execute_cmd(params, envp, &status);
	free_cmd(params.cmd1);
	free_cmd(params.cmd2);
	return (status);
}
