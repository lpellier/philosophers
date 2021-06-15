/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_destroy.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 15:01:05 by lpellier          #+#    #+#             */
/*   Updated: 2021/06/15 14:20:21 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

t_philo	create_philo(t_info *info, int index)
{
	t_philo		philo;

	philo.philo_number = index + 1;
	philo.info = info;
	philo.does = SLEEP;
	philo.number_of_meals = 0;
	if (philo.philo_number % 2 == 0)
		philo.does = FORK;
	return (philo);
}

t_philo	*init_philos(t_info *info)
{
	int			i;
	t_philo		*philos;

	i = 0;
	if (ft_calloc((void **)&philos, info->number_of_philosophers, \
		sizeof(t_philo)))
		return (NULL);
	while (i < info->number_of_philosophers)
	{
		philos[i] = create_philo(info, i);
		pthread_create(&philos[i].thread, NULL, philo_routine, &philos[i]);
		i++;
	}
	return (philos);
}

void	init_forks(t_info *info)
{
	int				i;

	i = 0;
	if (ft_calloc((void **)&info->forks, 1, sizeof(sem_t)))
		return ;
	if (ft_calloc((void **)&info->lock, 1, sizeof(sem_t)))
		return ;
	sem_unlink("forks");
	sem_unlink("lock");
	info->forks = sem_open("forks", O_CREAT, 00644, info->number_of_philosophers);
	info->lock = sem_open("lock", O_CREAT, 00644, 1);
}

void	join_philos(t_state *state)
{
	int		i;

	i = 0;
	while (i < state->info->number_of_philosophers)
	{
		pthread_join(state->philos[i].thread, NULL);
		i++;
	}
}

void	destroy_forks(t_state *state)
{
	int		i;

	i = 0;
	sem_close(state->info->forks);
	sem_close(state->info->lock);
	sem_unlink("forks");
	sem_unlink("lock");
}
