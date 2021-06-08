/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_destroy.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 15:01:05 by lpellier          #+#    #+#             */
/*   Updated: 2021/06/08 12:41:51 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	create_philo(t_info *info, pthread_mutex_t *forks, int index)
{
	t_philo		philo;

	philo.philo_number = index + 1;
	philo.adjacent_forks[0] = &forks[philo.philo_number - 1];
	philo.adjacent_forks[1] = &forks[philo.philo_number % info->number_of_philosophers];
	philo.info = info;
	philo.is_dead = FALSE;
	philo.does = SLEEP;
	philo.number_of_meals = 0;
	if (philo.philo_number % 2 == 0)
		philo.does = FORK;
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
	info->meal_goal = -1;
	info->everyone_is_alive = TRUE;
	gettimeofday(&info->time_since_start, NULL);
	if (av[5])
		info->meal_goal = ft_atoi(av[5]);
	state->info = info;
	state->forks = init_forks(info);
	state->philos = init_philos(info, state->forks);
	return (state);
}
