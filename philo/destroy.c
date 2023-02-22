/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 17:42:06 by lpellier          #+#    #+#             */
/*   Updated: 2021/11/30 15:30:02 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	secure_free(void **ptr)
{
	if (*ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void	destroy_forks(t_args args, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	while (i < args.nbr_of_philos)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	pthread_mutex_destroy(args.output_lock);
	secure_free((void **)&forks);
	secure_free((void **)&args.output_lock);
	secure_free((void **)&args.everyone_is_alive);
}

void	destroy_philos(t_args args, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < args.nbr_of_philos)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	secure_free((void **)&philos);
}
