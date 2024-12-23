/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgodet <rgodet@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 11:52:02 by rgodet            #+#    #+#             */
/*   Updated: 2024/12/23 14:06:36 by rgodet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static pid_t	create_fork(t_cmd cmd, int input_fd, int output_fd, char **envp)
{
	pid_t	cmd_pid;

	cmd_pid = fork();
	if (cmd_pid == 0)
	{
		dup2(input_fd, STDIN_FILENO);
		dup2(output_fd, STDOUT_FILENO);
		execve(cmd.path, cmd.args, envp);
		exit(log_error("Failed to execute command"));
	}
	return (cmd_pid);
}

static int	is_valid_command(t_cmd cmd, int input_fd, int output_fd)
{
	if (!cmd.path)
	{
		close(input_fd);
		close(output_fd);
		log_error("Command not found");
		return (0);
	}
	return (1);
}

static int	ft_input_to_temp_file(char *input)
{
	int		input_fd;
	int		temp_fd;
	char	buffer[MAX_BUFFER_SIZE];
	size_t	read_bytes;

	input_fd = open(input, O_RDONLY);
	temp_fd = open("/tmp/pipex_temp", O_CREAT | O_TRUNC | O_WRONLY, 0644);
	read_bytes = read(input_fd, buffer, MAX_BUFFER_SIZE);
	write(temp_fd, buffer, read_bytes);
	close(input_fd);
	close(temp_fd);
	return (open("/tmp/pipex_temp", O_RDONLY));
}

int	*ft_execute_cmd(t_params params, char **envp, int *status)
{
	int		pipe_fd[2];
	int		input_fd;
	int		output_fd;
	pid_t	cmd1_pid;
	pid_t	cmd2_pid;

	pipe(pipe_fd);
	input_fd = ft_input_to_temp_file(params.file1);
	output_fd = open(params.file2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	cmd1_pid = 0;
	cmd2_pid = 0;
	if (is_valid_command(params.cmd1, input_fd, pipe_fd[1]))
		cmd1_pid = create_fork(params.cmd1, input_fd, pipe_fd[1], envp);
	close(pipe_fd[1]);
	if (is_valid_command(params.cmd2, pipe_fd[0], output_fd))
		cmd2_pid = create_fork(params.cmd2, pipe_fd[0], output_fd, envp);
	close(pipe_fd[0]);
	if (cmd1_pid != 0)
		waitpid(cmd1_pid, NULL, 0);
	if (cmd2_pid != 0)
		waitpid(cmd2_pid, status, 0);
	close(input_fd);
	close(output_fd);
	return (status);
}
