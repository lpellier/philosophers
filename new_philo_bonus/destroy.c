/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 17:42:06 by lpellier          #+#    #+#             */
/*   Updated: 2021/12/02 14:32:21 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	secure_free(void **ptr)
{
	if (*ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void	destroy_forks(t_args args, sem_t *forks)
{
	sem_close(forks);
	sem_close(args.output_lock);
	sem_close(args.stop_program);
	secure_free((void **)&args.everyone_is_alive);
	sem_unlink("forks");
	sem_unlink("stop");
	sem_unlink("output");
}

void    kill_all_processes(t_philo *philos)
{
	int             i;

	i = 0;
	while (i < philos[0].args.nbr_of_philos)
	{
		kill(philos[i].cpid, SIGKILL);
		i++;
	}
}

void	destroy_philos(t_args args, t_philo *philos)
{
	int	i;
	int	status;
	
	i = 0;
	(void)args;
	status = 0;
	while (1) {
		waitpid(-1, &status, 0);
		if (status) {
			kill_all_processes(philos);
			break ;
		}
	}
	secure_free((void **)&philos);
}
