/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:26:35 by camerico          #+#    #+#             */
/*   Updated: 2025/03/19 14:03:09 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//fichier pour la fonction principale qui initialise tout

int	main(int argc, char **argv, char **envp)
{
	int	infile;
	int	outfile;
	int	pipefd[2];
	
	if (argc != 5)
	{
		ft_printf("Error : Wrong number of arguments");
		return (0);
	}
	
	infile = open(argv[1], O_RDONLY);
	if (infile == -1)
	{
		perror("Error opening infile");
		return (1);
	}
	
	outfile = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644); // O_TRUNC -> pour vider si le fichier ne l'est pas
	if (outfile == -1)
	{
		perror("Error opening outfile");
		return (1);
	}
	
	//creation du pipe (tube)
	if (pipe(pipefd) == -1)
	{
		perror("Error pipe");
		close (infile);
		close (outfile);
		return (1);
	}
	exec_cmd1(infile, pipefd, envp, argv[2]);
	exec_cmd2(outfile, pipefd, envp, argv[3]);
	
//	on ferme tous les fichiers dans le process parent
	close(pipefd[0]);
	close(pipefd[1]);
	close(infile);
	close(outfile);

	wait(NULL); //permet d'attendre que le processus enfant a bien fini de s'ecxecuter avant de permet
	wait(NULL);
}
