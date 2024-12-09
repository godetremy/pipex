/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_arguments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgodet <rgodet@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 10:01:52 by rgodet            #+#    #+#             */
/*   Updated: 2024/12/09 13:53:56 by rgodet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static void  log_file_error(char *file, char *msg)
{
	char	*log_msg;

	log_msg = ft_strjoin(file, msg);
	log_error(log_msg);
	free(log_msg);
	exit(1);
}

static void  check_file(char *file, int exist, int read, int write)
{
	t_file_access file_access;

	file_access = check_file_access(file);
	if (!file_access.file_exist && exist)
		log_file_error(file, " does not exist");
	if (!file_access.can_read && read)
		log_file_error(file, " is not readable");
	if (!file_access.can_write && write && file_access.file_exist)
		log_file_error(file, " is not writable");
}

t_params	check_arguments(int argc, char **argv)
{
	t_params	params;

	if (argc != 5)
		exit(log_error("Invalid number of arguments\nUsage: ./pipex file1 cmd1 cmd2 file2"));
	params.file1 = argv[1];
	params.cmd1 = argv[2];
	params.cmd2 = argv[3];
	params.file2 = argv[4];
	check_file(params.file1, 1, 1, 0);
	check_file(params.file2, 0, 0, 1);
	return (params);
}