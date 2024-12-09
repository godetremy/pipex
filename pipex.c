/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgodet <rgodet@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 09:05:29 by rgodet            #+#    #+#             */
/*   Updated: 2024/12/09 15:20:06 by rgodet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

#include <unistd.h>
#include <stdlib.h>

#include <stdio.h>
int	main(int argc, char **argv)
{
	t_params params;

	params = check_arguments(argc, argv);
	(void)params;

	t_cmd cmd1;
	cmd1.path = "/bin/cat";
	cmd1.args = (char *[]){"cat", NULL};
	t_cmd cmd2;
	cmd2.path = "/usr/bin/wc";
	cmd2.args = (char *[]){"wc", "-l", NULL};

	ft_execute_cmd(cmd1, cmd2);
	return (0);
}