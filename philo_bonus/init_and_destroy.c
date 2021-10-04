/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_destroy.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 15:01:05 by lpellier          #+#    #+#             */
/*   Updated: 2021/10/04 11:39:02 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_philo	create_philo(t_info *info, int index)
{
	t_philo		philo;

	philo.philo_number = index + 1;
	philo.info = info;
	philo.does = SLEEP;
	philo.number_of_meals = 0;
	philo.is_alive = true;
	if (philo.philo_number % 2 == 0)
		philo.does = FORK;
	return (philo);
}

t_philo	*init_philos(t_info *info)
{
	int			i;
	t_philo		*philos;
	t_process	*processes;

	if (ft_calloc((void **)&philos, info->nbr_philo, sizeof(t_philo)))
		return (NULL);
	if (ft_calloc((void **)&processes, info->nbr_philo, sizeof(t_process)))
		return (NULL);
	i = 0;
	while (i < info->nbr_philo)
	{
		philos[i] = create_philo(info, i);
		processes[i].philo_index = i;
		processes[i].nbr_of_philos = info->nbr_philo;
		i++;
	}
	i = 0;
	while (i < info->nbr_philo)
	{
		processes[i].philos = philos;
		pthread_create(&philos[i].thread, NULL, philo_process, &processes[i]);
		i++;
	}
	return (philos);
}

void	init_forks(t_state *state)
{
	int				i;

	i = 0;
	sem_unlink("forks");
	sem_unlink("lock");
	state->forks = sem_open("forks", O_CREAT, 00644, \
		state->info->nbr_philo);
	state->lock = sem_open("lock", O_CREAT, 00644, 1);
}

void	join_philos(t_state *state)
{
	int		i;

	i = 0;
	while (i < state->info->nbr_philo)
	{
		pthread_join(state->philos[i].thread, NULL);
		kill(state->philos[i].cpid, SIGINT);
		i++;
	}
}

void	destroy_forks(t_state *state)
{
	int		i;

	i = 0;
	sem_close(state->forks);
	sem_close(state->lock);
	sem_unlink("forks");
	sem_unlink("lock");
}
