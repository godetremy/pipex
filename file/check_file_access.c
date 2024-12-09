/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_file_access.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgodet <rgodet@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 11:29:34 by rgodet            #+#    #+#             */
/*   Updated: 2024/12/09 11:30:28 by rgodet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"
t_file_access	check_file_access(char *file)
{
	t_file_access file_access;

	file_access.file_exist = access(file, F_OK) == 0;
	file_access.can_read = access(file, R_OK) == 0;
	file_access.can_write = access(file, W_OK) == 0;
	file_access.can_execute = access(file, X_OK) == 0;
	return (file_access);
}