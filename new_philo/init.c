/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 17:42:44 by lpellier          #+#    #+#             */
/*   Updated: 2021/11/26 19:00:05 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

pthread_mutex_t	*init_forks(t_args args)
{
	pthread_mutex_t	*forks;
	int				i;

	i = 0;
	forks = malloc(sizeof(pthread_mutex_t) * args.nbr_of_philos);
	if (!forks)
		return (NULL);
	while (i < args.nbr_of_philos)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	return (forks);
}

t_philo	create_philo(t_args args, pthread_mutex_t *forks, int index)
{
	t_philo	philo;

	philo.philo_index = index + 1;
	philo.adjacent_forks[0] = &forks[philo.philo_index - 1];
	philo.adjacent_forks[1] = &forks[philo.philo_index % args.nbr_of_philos];
	philo.does = SLEEP;
	if (philo.philo_index % 2 == 0)
		philo.does = FORK;
	philo.holding_forks = 0;
	philo.args.nbr_of_philos = args.nbr_of_philos;
	philo.args.time_to_die = args.time_to_die;
	philo.args.time_to_eat = args.time_to_eat;
	philo.args.time_to_sleep = args.time_to_sleep;
	philo.args.meal_goal = args.meal_goal;
	philo.args.everyone_is_alive = args.everyone_is_alive;
	philo.args.output_lock = args.output_lock;
	philo.meals_eaten = 0;
	return (philo);
}

t_philo	*init_philos(t_args args, pthread_mutex_t *forks)
{
	t_philo	*philos;
	int	index;

	index = 0;
	philos = malloc(sizeof(t_philo) * args.nbr_of_philos);
	if (!philos)
		return (NULL);
	while (index < args.nbr_of_philos)
	{
		philos[index] = create_philo(args, forks, index);
		pthread_create(&philos[index].thread, NULL, philo_routine, &philos[index]);
		index++;
	}
	return (philos);
}
