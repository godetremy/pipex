/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgodet <rgodet@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 11:52:02 by rgodet            #+#    #+#             */
/*   Updated: 2025/01/15 16:57:58 by rgodet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static pid_t	create_fork_in(t_cmd cmd, char *infile, int pipe[2], char **envp)
{
	pid_t	cmd_pid;
	int		infile_fd;

	cmd_pid = fork();
	if (cmd_pid == 0)
	{
		infile_fd = open(infile, O_RDONLY);
		dup2(infile_fd, STDIN_FILENO);
		dup2(pipe[1], STDOUT_FILENO);
		close(infile_fd);
		close(pipe[0]);
		close(pipe[1]);
		execve(cmd.path, cmd.args, envp);
		exit(log_error("Failed to execute command"));
	}
	return (cmd_pid);
}

static pid_t	create_fork_out(t_cmd cmd, int pipe[2], char *outfile, char **envp)
{
	pid_t	cmd_pid;
	int		outfile_fd;

	cmd_pid = fork();
	if (cmd_pid == 0)
	{
		outfile_fd = open(outfile, O_CREAT | O_TRUNC | O_WRONLY, 0644);
		dup2(outfile_fd, STDOUT_FILENO);
		dup2(pipe[0], STDIN_FILENO);
		close(outfile_fd);
		close(pipe[0]);
		close(pipe[1]);
		execve(cmd.path, cmd.args, envp);
		exit(log_error("Failed to execute command"));
	}
	return (cmd_pid);
}

#include <stdio.h>
static int	is_valid_command(t_cmd cmd, int pipe_fd)
{
	if (!cmd.path)
	{
		close(pipe_fd);
		log_error("Command not found");
		return (0);
	}
	return (1);
}

static void	ft_input_to_temp_file(char *input)
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
}

static int	ft_next_cmd_part2(int cmd2_pid,
								t_params params)
{
	int	status;

	status = 0;
	if (cmd2_pid != 0)
		waitpid(cmd2_pid, &status, 0);
	else
	{
		if (params.file2 == NULL)
			status = 1;
		else
			status = 127;
	}
	return (status);
}

int	*ft_execute_cmd(t_params params, char **envp, int *status)
{
	int		pipe_fd[2];
	pid_t	cmd1_pid;
	pid_t	cmd2_pid;

	pipe(pipe_fd);
	if (params.file1)
    {
		ft_input_to_temp_file(params.file1);
		cmd1_pid = 0;
		if (is_valid_command(params.cmd1, pipe_fd[1]) && params.file1)
			cmd1_pid = create_fork_in(params.cmd1, params.file1, pipe_fd, envp);
    }
	cmd2_pid = 0;
	close(pipe_fd[1]);
	if (is_valid_command(params.cmd2, pipe_fd[0]) && params.file2)
		cmd2_pid = create_fork_out(params.cmd2, pipe_fd, params.file2, envp);
	close(pipe_fd[0]);
	if (cmd1_pid != 0)
		waitpid(cmd1_pid, NULL, 0);
	*status = ft_next_cmd_part2(cmd2_pid, params);
	return (status);
}
