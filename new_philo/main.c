/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 17:19:49 by lpellier          #+#    #+#             */
/*   Updated: 2021/11/29 18:23:48 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*check_time(void *arg)
{
	t_philo	*philo;
	int	everyone_alive;

	philo = arg;
	everyone_alive = 1;
	while (everyone_alive)
	{
		pthread_mutex_lock(philo->args.output_lock);
		if (!everyone_alive || philo->meals_eaten == philo->args.meal_goal)
		{
			pthread_mutex_unlock(philo->args.output_lock);
			return (NULL);
		}
		if (time_passed(&philo->time_since_last_meal) > philo->args.time_to_die)
		{
			if (!everyone_alive || philo->meals_eaten == philo->args.meal_goal)
			{
				pthread_mutex_unlock(philo->args.output_lock);
				return (NULL);
			}
			pthread_mutex_unlock(philo->args.output_lock);
			output(philo, "has died");
			pthread_mutex_lock(philo->args.output_lock);
			*(philo->args.everyone_is_alive) = 0;
			pthread_mutex_unlock(philo->args.output_lock);
			return (NULL);
		}
		everyone_alive = *(philo->args.everyone_is_alive);
		pthread_mutex_unlock(philo->args.output_lock);
	}
	return (NULL);
}

void	*philo_routine(void *arg)
{
	pthread_t	timer;
	t_philo		*philo;
	int			everyone_alive;

	philo = (t_philo *)arg;
	gettimeofday(&philo->time_since_last_meal, NULL);
	pthread_mutex_lock(philo->args.output_lock);
	pthread_create(&timer, NULL, &check_time, (void *)philo);
	pthread_mutex_unlock(philo->args.output_lock);
	everyone_alive = 1;
	while (everyone_alive)
	{
		philo_does(philo);
		if (philo->args.meal_goal != -1 && \
			philo->meals_eaten >= philo->args.meal_goal)
			break;
		pthread_mutex_lock(philo->args.output_lock);
		everyone_alive = *(philo->args.everyone_is_alive);
		pthread_mutex_unlock(philo->args.output_lock);
	}
	if (philo->holding_forks >= 1)
		pthread_mutex_unlock(philo->adjacent_forks[1]);
	if (philo->holding_forks >= 2)
		pthread_mutex_unlock(philo->adjacent_forks[0]);
	philo->holding_forks = 0;
	if (everyone_alive && philo->args.meal_goal != -1)
		output(philo, "is done");
	pthread_join(timer, NULL);
	return (NULL);
}

int	main(int ac, char **av)
{
	t_args			args;
	t_philo			*philos;
	pthread_mutex_t	*forks;

	if ((ac != 5 && ac != 6) || error_in_args(av) || fill_args(av, &args))
	{
		printf("arg error\nnumber_of_philosophers | ");
		printf("time_to_die | time_to_eat | time_to_sleep | ");
		printf("[number_of_time_each_philosopher_must_eat]\n");
		exit(EXIT_FAILURE);
	}
	forks = init_forks(args);
	if (!forks)
		exit(EXIT_FAILURE);
	philos = init_philos(args, forks);
	if (!philos)
	{
		destroy_forks(args, forks);
		exit(EXIT_FAILURE);
	}
	destroy_philos(args, philos);
	destroy_forks(args, forks);
}
