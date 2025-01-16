/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_arguments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgodet <rgodet@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 10:01:52 by rgodet            #+#    #+#             */
/*   Updated: 2025/01/16 09:37:26 by rgodet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static int	check_file(char *file, int exist, int read, int write)
{
	t_file_access	file_access;

	file_access = check_file_access(file);
	if (!file_access.file_exist && exist)
		set_error(2, file);
	else if ((!file_access.can_read && read && file_access.file_exist)
		|| (!file_access.can_write && write && file_access.file_exist))
		set_error(13, file);
	return ((!file_access.file_exist && exist)
		|| (!file_access.can_read && read && file_access.file_exist)
		|| (!file_access.can_write && write && file_access.file_exist));
}

t_params	check_arguments(int argc, char **argv)
{
	t_params	params;

	if (argc != 5)
	{
		set_error(22, "pipex");
		exit(1);
	}
	params.file1 = argv[1];
	params.cmd1_str = argv[2];
	params.cmd2_str = argv[3];
	params.file2 = argv[4];
	if (check_file(params.file1, 1, 1, 0))
		params.file1 = NULL;
	if (check_file(params.file2, 0, 0, 1))
		params.file2 = NULL;
	return (params);
}
