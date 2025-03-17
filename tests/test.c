/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:04:39 by camerico          #+#    #+#             */
/*   Updated: 2025/03/17 14:44:55 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>  // Pour open()

//fonction test pour creer un pipe
int	main(void)
{
	int	fd[2];
	char message[] = "Hello pipe";
	char buffer[20];
	
	if (pipe(fd) == -1)
	{
		perror("Erreur pipe");
		return (1);
	}
	
	write(fd[1], message, strlen(message) + 1); //on ecrit dans le pipe
	read(fd[0], buffer, sizeof(buffer)); // on lit depuis le pipe
	
	printf("Message recu : %s\n", buffer);
	return (0);
}



//fonction test pour utiliser fork()
int	main(void)
{
	__pid_t	pid = fork(); //creation d'un processus enfant
	
	if (pid > 0) // processus parents
		printf("Je suis le parent, mon PID est %d\n", getpid());
	else if (pid == 0)
		printf("Je suis l'enfant, mon PID est %d\n", getpid());
	else
	{
		perror("Erreur de fork");
		return (1);
	}
	return (0);
}


//fonction test pour utiliser dup(2)
int	main(void)
{
	int	file;

	file = open("mon_fichier.txt", O_WRONLY); //on peut ajouter O_TRUNC pour vider le fichier si il n'est pas vide
	
	if (file == -1)
	{
		perror("Erreur ouverture ficher");
		return (1);
	}
	
	//on redirige stdout vers le fichier
	dup2(file, STDOUT_FILENO);
	close (file);

	//Tout ce qui suit sera ecrit dans mon_fichier
	printf("Hello, fichier\n");
	write(1, "Ceci est une autre ligne\n", 25); //1 = STDOUT_FILENO

	return (0);
}