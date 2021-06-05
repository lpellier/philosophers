/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_destroy.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 15:01:05 by lpellier          #+#    #+#             */
/*   Updated: 2021/06/05 16:58:32 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	create_philo(t_info *info, pthread_mutex_t *forks, int index)
{
	t_philo		philo;

	if (index == 0)
	{
		philo.adjacent_forks[0] = &forks[info->number_of_philosophers - 1];
		philo.adjacent_forks[1] = &forks[0];
	}
	else if (index == info->number_of_philosophers)
	{
		philo.adjacent_forks[0] = &forks[0];
		philo.adjacent_forks[1] = &forks[info->number_of_philosophers - 1];
	}
	else
	{
		philo.adjacent_forks[0] = &forks[index];
		philo.adjacent_forks[1] = &forks[index + 1];
	}
	philo.philo_number = index + 1;
	philo.info = info;
	return (philo);
}

t_philo	*init_philos(t_info *info, pthread_mutex_t *forks)
{
	int			i;
	t_philo		*philos;

	i = 0;
	if (ft_calloc((void **)&philos, info->number_of_philosophers, \
		sizeof(t_philo)))
		return (NULL);
	while (i < info->number_of_philosophers)
	{
		philos[i] = create_philo(info, forks, i);
		pthread_create(&philos[i].thread, NULL, philo_routine, &philos[i]);
		i++;
	}
	return (philos);
}

pthread_mutex_t	*init_forks(t_info *info)
{
	int				i;
	pthread_mutex_t	*forks;

	i = 0;
	if (ft_calloc((void **)&forks, info->number_of_philosophers, \
		sizeof(pthread_mutex_t)))
		return (NULL);
	while (i < info->number_of_philosophers)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	return (forks);
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
	while (i < state->info->number_of_philosophers)
	{
		pthread_mutex_destroy(&state->forks[i]);
		i++;
	}
}

t_state	*init_state(char **av)
{
	t_state	*state;
	t_info	*info;

	if ((ft_calloc((void **)&state, 1, sizeof(t_state))))
		return (NULL);
	if ((ft_calloc((void **)&info, 1, sizeof(t_info))))
		return (NULL);
	info->number_of_philosophers = ft_atoi(av[1]);
	info->time_to_die = ft_atoi(av[2]);
	info->time_to_eat = ft_atoi(av[3]);
	info->time_to_sleep = ft_atoi(av[4]);
	info->number_of_time_each_philosopher_must_eat = -1;
	gettimeofday(&info->time_since_start, NULL);
	if (av[5])
		info->number_of_time_each_philosopher_must_eat = ft_atoi(av[5]);
	state->info = info;
	state->forks = init_forks(info);
	state->philos = init_philos(info, state->forks);
	return (state);
}
