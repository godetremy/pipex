/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgodet <rgodet@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 13:45:11 by rgodet            #+#    #+#             */
/*   Updated: 2024/12/09 17:03:25 by rgodet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"
#include <stdio.h>

static pid_t  create_fork(t_cmd cmd, int *pipe_fd)
{
    pid_t cmd_pid;

    cmd_pid = fork();
    if (cmd_pid == 0) {
       dup2(pipe_fd[1], STDOUT_FILENO);
       close(pipe_fd[1]);
       close(pipe_fd[0]);
       execv(cmd.path, cmd.args);
       exit(log_error("Failed to execute command"));
    }
    return (cmd_pid);
}

int ft_execute_cmd(t_cmd cmd1, t_cmd cmd2)
{
    int pipe_fd[2];

    pipe(pipe_fd);

    write(pipe_fd[1], "Hello, world!\n", 14);
    close(pipe_fd[1]);
    pid_t cmd1_pid = create_fork(cmd1, pipe_fd);
    //pid_t cmd2_pid = create_fork(cmd2, pipe_fd);
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    waitpid(cmd1_pid, NULL, 0);
    //waitpid(cmd2_pid, NULL, 0);
    (void)cmd2;

    char *buf = malloc(1024);
    int bytes_read = read(pipe_fd[1], buf, 1024);  // Read the output from pipe
    if (bytes_read > 0) {
       buf[bytes_read] = '\0';  // Ensure the output is null-terminated
       printf("Output: %s", buf);
    } else {
       printf("No output from the pipe\n");
    }

    free(buf);
    return 0;
}