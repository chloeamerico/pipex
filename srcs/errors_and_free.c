/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_and_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:50:38 by camerico          #+#    #+#             */
/*   Updated: 2025/03/25 19:11:52 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	all_close(int pipefd[2], t_files *files)
{
	close(files->infile);
	close(files->outfile);
	close(pipefd[0]);
	close(pipefd[1]);
}

void	cmd_not_found(char **cmd_arg, t_files *files, char *cmd_name)
{
	ft_putstr_fd("Command ", STDERR_FILENO);
	ft_putstr_fd(cmd_name, STDERR_FILENO);
	ft_putstr_fd(" not found\n", STDERR_FILENO);
	free_tab(cmd_arg);
	close(files->infile);
	close(files->outfile);
	exit(127);
}
