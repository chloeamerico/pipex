/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:27:11 by camerico          #+#    #+#             */
/*   Updated: 2025/03/27 17:57:54 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	exec_cmd1(t_files *files, int pipefd[2], char **envp, char *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (perror("Fork failed"), -1);
	if (pid == 0)
	{
		if (dup2(files->infile, STDIN_FILENO) == -1)
		{
			perror("dup2 error to redirect stdin to infile");
			all_close(pipefd, files);
			exit(1);
		}
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 error to redirect stdout to pipe");
			all_close(pipefd, files);
			exit(1);
		}
		close(pipefd[0]);
		close(pipefd[1]);
		execute_cmd(cmd, envp, files, pipefd);
	}
	return (pid);
}

int	exec_cmd2(t_files *files, int pipefd[2], char **envp, char *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (perror("Fork failed"), -1);
	if (pid == 0)
	{
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			perror("dup2 error to redirect stdin to infile");
			all_close(pipefd, files);
			exit(1);
		}
		if (dup2(files->outfile, STDOUT_FILENO) == -1)
		{
			perror("dup2 error to redirect stdout to pipe");
			all_close(pipefd, files);
			exit(1);
		}
		close(pipefd[0]);
		close(pipefd[1]);
		execute_cmd(cmd, envp, files, pipefd);
	}
	return (pid);
}

int	execute_cmd(char *cmd, char **envp, t_files *files, int pipefd[2])
{
	char	**cmd_arg;
	char	*cmd_path;

	if (!cmd)
	{
		all_close(pipefd, files);	
		exit(1);
	}
	cmd_arg = ft_split(cmd, ' ');
	if (!cmd_arg || !cmd_arg[0])
	{
		ft_printf("Error : invalid command");
		all_close(pipefd, files);
		free_tab(cmd_arg);
		exit (1);
	}
	cmd_path = find_cmd_path(cmd_arg[0], envp);
	if (!cmd_path)
		cmd_not_found(cmd_arg, files, cmd_arg[0]);
	execve(cmd_path, cmd_arg, envp);
	perror("execve failed");
	free_tab(cmd_arg);
	free(cmd_path);
	all_close(pipefd, files);
	exit(1);
}

// cette fonction cherche le PATH parmi toutes les var d'env de envp
// puis teste toutes les diff chemin possibles 
//pour trouver le chemin de la commande 
char	*find_cmd_path(char *cmd, char **envp)
{
	int		i;
	char	**paths;
	char	*tmp;
	char	*full_path;

	if (access(cmd, X_OK) == 0)
		return(cmd);
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = (ft_split(envp[i] + 5, ':'));
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		if (access(full_path, X_OK) == 0)
			return (free(tmp), free(paths), full_path);
		free(full_path);
		free(tmp);
		i++;
	}
	free_tab(paths);
	return (NULL);
}
