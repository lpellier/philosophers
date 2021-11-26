/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_destroy.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 15:01:05 by lpellier          #+#    #+#             */
/*   Updated: 2021/11/26 16:39:42 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	create_philo(t_info *info, pthread_mutex_t *forks, int index)
{
	t_philo		philo;

	philo.philo_number = index + 1;
	philo.adjacent_forks[0] = &forks[philo.philo_number - 1];
	philo.adjacent_forks[1] = \
		&forks[philo.philo_number % info->number_of_philosophers];
	philo.info = info;
	philo.does = SLEEP;
	philo.holding = 0;
	philo.time_to_die = info->time_to_die;
	philo.time_to_eat = info->time_to_eat;
	philo.time_to_sleep = info->time_to_sleep;
	philo.meal_goal = info->meal_goal;
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
		pthread_mutex_lock(&info->var_lock);
		pthread_create(&philos[i].thread, NULL, philo_routine, &philos[i]);
		pthread_mutex_unlock(&info->var_lock);
		i++;
	}
	return (philos);
}

pthread_mutex_t	*init_forks(t_info *info)
{
	int				i;
	pthread_mutex_t	*forks;

	i = 0;
	pthread_mutex_init(&info->output_lock, NULL);
	pthread_mutex_init(&info->var_lock, NULL);
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
	pthread_mutex_destroy(&state->info->output_lock);
	pthread_mutex_destroy(&state->info->var_lock);
	while (i < state->info->number_of_philosophers)
	{
		pthread_mutex_destroy(&state->forks[i]);
		i++;
	}
}
