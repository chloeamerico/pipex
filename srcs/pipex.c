/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:26:35 by camerico          #+#    #+#             */
/*   Updated: 2025/03/27 17:03:35 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_files	files;
	int		pipefd[2];

	if (!envp || !*envp)
		return (ft_printf("Error : There is no environment\n"), 1);
	if (argc != 5)
		return (ft_printf("Error : Wrong number of arguments"), 0);
	if (init_pipe(argv, &files, pipefd))
		return (1);
	if (children(&files, pipefd, envp, argv))
		return (1);
	return (0);
}

int	init_pipe(char **argv, t_files *files, int pipefd[2])
{
	files->infile = open(argv[1], O_RDONLY);
	if (files->infile == -1)
		return (perror("Error opening infile"), 1);
	files->outfile = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (files->outfile == -1)
	{
		perror("Error opening outfile");
		close (files->infile);
		return (1);
	}
	if (pipe(pipefd) == -1)
	{
		perror("Error pipe");
		close (files->infile);
		close (files->outfile);
		return (1);
	}
	return (0);
}

int	children(t_files *files, int pipefd[2], char **envp, char **argv)
{
	int	pid1;
	int	pid2;
	int	status1;
	int	status2;

	pid1 = exec_cmd1(files, pipefd, envp, argv[2]);
	if (pid1 == -1)
		return (perror("exec_cmd1 failed"), all_close(pipefd, files), 1);
	pid2 = exec_cmd2(files, pipefd, envp, argv[3]);
	if (pid2 == -1)
	{
		perror("exec_cmd2 failed");
		all_close(pipefd, files);
		return (1);
	}
	all_close(pipefd, files);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status1) && WEXITSTATUS(status1) != 0)
		ft_printf("cmd1 failed with code : %d\n", WEXITSTATUS(status1));
	if (WIFEXITED(status2) && WEXITSTATUS(status2) != 0)
		ft_printf("cmd2 failed with code : %d\n", WEXITSTATUS(status2));
	return (0);
}
