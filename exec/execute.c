/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgodet <rgodet@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 11:52:02 by rgodet            #+#    #+#             */
/*   Updated: 2025/01/16 17:43:57 by rgodet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static pid_t	create_fork_in(t_cmd cmd, char *infile, int pipe[2],
								char **envp)
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
		exit(set_error(8, cmd.path));
	}
	return (cmd_pid);
}

static pid_t	create_fork_out(t_params params, int pipe[2], char *outfile,
								char **envp)
{
	pid_t	cmd_pid;
	int		outfile_fd;

	cmd_pid = fork();
	if (cmd_pid == 0)
	{
		outfile_fd = open(outfile, O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if (outfile_fd == -1)
		{
			free_cmd(params.cmd1);
			free_cmd(params.cmd2);
			close(pipe[0]);
			close(pipe[1]);
			exit(set_error(errno, outfile));
		}
		dup2(outfile_fd, STDOUT_FILENO);
		dup2(pipe[0], STDIN_FILENO);
		close(outfile_fd);
		close(pipe[0]);
		close(pipe[1]);
		execve(params.cmd2.path, params.cmd2.args, envp);
		free_cmd(params.cmd1);
		free_cmd(params.cmd2);
		exit(set_error(errno, params.cmd2.path));
	}
	return (cmd_pid);
}

static int	is_valid_command(t_cmd cmd, int pipe_fd, char *outfile)
{
	int		outfile_fd;

	if (!cmd.path)
	{
		close(pipe_fd);
		if (outfile)
		{
			outfile_fd = open(outfile, O_CREAT | O_TRUNC | O_WRONLY, 0644);
			close(outfile_fd);
		}
		set_error(2, cmd.args[0]);
		return (0);
	}
	return (1);
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
	cmd1_pid = 0;
	if (is_valid_command(params.cmd1, pipe_fd[1], NULL) && params.file1)
		cmd1_pid = create_fork_in(params.cmd1, params.file1, pipe_fd, envp);
	cmd2_pid = 0;
	close(pipe_fd[1]);
	if (is_valid_command(params.cmd2, pipe_fd[0], params.file2) && params.file2)
		cmd2_pid = create_fork_out(params, pipe_fd, params.file2, envp);
	close(pipe_fd[0]);
	if (cmd1_pid != 0)
		waitpid(cmd1_pid, NULL, 0);
	*status = ft_next_cmd_part2(cmd2_pid, params);
	return (status);
}
