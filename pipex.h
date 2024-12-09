/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgodet <rgodet@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 09:05:17 by rgodet            #+#    #+#             */
/*   Updated: 2024/12/09 15:02:34 by rgodet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
#define PIPEX_H

#include "libft/libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

struct s_params
{
	char	*file1;
	char	*cmd1;
	char	*cmd2;
	char	*file2;
};
typedef struct s_params	t_params;

struct s_file_access
{
	int		file_exist;
	int		can_read;
	int		can_write;
	int		can_execute;
};
typedef struct s_file_access	t_file_access;

struct s_cmd
{
    char	*path;
    char	**args;
};
typedef struct s_cmd	t_cmd;

t_params		check_arguments(int argc, char **argv);
t_file_access	check_file_access(char *file);
int				log_error(char *msg);
int				ft_execute_cmd(t_cmd cmd1, t_cmd cmd2);

#endif //PIPEX_H