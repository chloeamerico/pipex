/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:04:25 by camerico          #+#    #+#             */
/*   Updated: 2025/03/25 19:59:18 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

/* ************************************************************************** */
/* INCLUDES                                                                   */
/* ************************************************************************** */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <strings.h>
# include <unistd.h>
# include <limits.h>
# include <stdbool.h>
# include "./libft/includes/libft.h"
# include <fcntl.h> 
# include <sys/wait.h>

/* ************************************************************************** */
/* DEFINES                                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* STRUCTURES                                                                 */
/* ************************************************************************** */

typedef struct s_files
{
	int	infile;
	int	outfile;
}	t_files;

/* ************************************************************************** */
/* PROTO                                                                      */
/* ************************************************************************** */

void	free_tab(char **cmd_arg);
int		exec_cmd1(t_files *files, int pipefd[2], char **envp, char *cmd);
int		exec_cmd2(t_files *files, int pipefd[2], char **envp, char *cmd);
int		execute_cmd(char *cmd, char **envp, t_files *files, int pipefd[2]);
char	*find_cmd_path(char *cmd, char **envp);
void	all_close(int pipefd[2], t_files *files);
void	cmd_not_found(char **cmd_arg, t_files *files, char *cmd_name);
int		children(t_files *files, int pipefd[2], char **envp, char **argv);
int		init_pipe(char **argv, t_files *files, int pipefd[2]);

/* ************************************************************************** */
/* DEBUG                                                                      */
/* ************************************************************************** */

#endif