/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:27:11 by camerico          #+#    #+#             */
/*   Updated: 2025/03/19 16:53:37 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//fonction qui execute une commande en utilisant execve()

// creer un processus enfant avec fork
// on redirige les entrees/sortie : infile deviens STDIN et pipefd[1] devient le STDOUT
// on execute la commande 1 avec execve()
// on gere les erreurs et fermeture des fd

int	exec_cmd1(int infile, int pipefd[2], char **envp, char *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("Fork failed");
		return (1);
	}
	
	if (pid == 0) // cad qu'on est bien dans le processus enfant
	{
		if (dup2(infile, STDIN_FILENO) == -1)
		{
			perror("dup2 error to redirect stdin to infile");
			exit(1);
		}
		
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 error to redirect stdout to pipe");
			exit(1);
		}
		close(pipefd[0]);
		close(infile);
		close(pipefd[1]);

		execute_cmd(cmd, envp, pipefd);
	}
	return(0);
}

int	exec_cmd2(int outfile, int pipefd[2], char **envp, char *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("Fork failed");
		return (1);
	}
	
	if (pid == 0) // cad qu'on est bien dans le processus enfant
	{
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			perror("dup2 error to redirect stdin to infile");
			exit(1);
		}
		
		if (dup2(outfile, STDOUT_FILENO) == -1)
		{
			perror("dup2 error to redirect stdout to pipe");
			exit(1);
		}
		close(pipefd[0]);
		close(outfile);
		close(pipefd[1]);
		
		execute_cmd(cmd, envp, pipefd);
		exit(1);
	}
	return(0);
}

void	execute_cmd(char *cmd, char **envp, int pipefd[2])
{
	char	**cmd_arg; //tab qui va contenir les arguments de la commande
	char	*cmd_path; // chmin de la commande
	
	cmd_arg = ft_split(cmd, ' ');

	if (!cmd_arg)
	{
		ft_printf("Error : invalid command");
		exit (1);
	}
	cmd_path = find_cmd_path(cmd_arg[0], envp);
	if (!cmd_path)
	{
		ft_putstr_fd("Command ", STDERR_FILENO);
		ft_putstr_fd(cmd_arg[0], STDERR_FILENO);
		ft_putstr_fd(" not found\n", STDERR_FILENO);
		free_tab(cmd_arg);
		exit(127); // indique que la commande est introuvable
	}
	// on execute la commande
	if(!execve(cmd_path, cmd_arg, envp))
	{
		close (pipefd[0]);
		close (pipefd[1]);
	}
	free_tab(cmd_arg);
	exit(1);
}


// cette fonction cherche le PATH parmi toutes les var d'env de envp
// puis teste toutes les diff chemin possibles pour trouver le chemin de la commande 
char	*find_cmd_path(char *cmd, char **envp)
{
	int	i;
	char	**paths;
	char	*tmp;
	char	*full_path;

	i = 0;
	while(envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = (ft_split(envp[i] + 5, ':'));
	
	i = 0;
	while(paths[i])
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
