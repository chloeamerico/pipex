/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_and_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:50:38 by camerico          #+#    #+#             */
/*   Updated: 2025/03/18 16:52:36 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_cmd_arg(char **cmd_arg)
{
	int	i;

	i = 0;
	while (cmd_arg[i])
	{
		free(cmd_arg[i]);
		i++;
	}
	free(cmd_arg);
}