/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 16:26:35 by lpellier          #+#    #+#             */
/*   Updated: 2021/06/04 16:16:32 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    secure_free(void *ptr)
{
	if (ptr)
		free(ptr);
	ptr = NULL;
}

long	time_passed(struct timeval ref, struct timeval *timer)
{
	gettimeofday(timer, NULL);
	return (((timer->tv_sec - ref.tv_sec) * 1000) + \
		((timer->tv_usec - ref.tv_usec) / 1000));
}

// void	*check_time(void *arg)
// {
// 	struct timeval timer;
// 	struct timeval *time_since_start;
	
// 	time_since_start = (struct timeval *)arg;
// 	timer.tv_sec = 0;
// 	timer.tv_usec = 0;
// 	while (timer.t)
// }

void	*philo_routine(void *arg)
{
	t_philinfo *philinfo;
	struct timeval timer;
	// pthread_t	timer;
	int			dead;

	dead = 0;
	philinfo = (t_philinfo *)(arg);
	while (!dead)
	{
		printf("%ld %d is thinking\n", time_passed(philinfo->state->time_since_start, &timer), philinfo->philo_number);
		// pthread_create(&timer, NULL, &check_time, &philinfo->state->time_since_start);
		pthread_mutex_lock(&philinfo->adjacent_forks[0]);
		pthread_mutex_lock(&philinfo->adjacent_forks[1]);
		usleep(philinfo->state->time_to_eat * 1000);
		printf("%ld %d is eating\n", time_passed(philinfo->state->time_since_start, &timer), philinfo->philo_number);
		pthread_mutex_unlock(&philinfo->adjacent_forks[0]);
		pthread_mutex_unlock(&philinfo->adjacent_forks[1]);
		usleep(philinfo->state->time_to_sleep * 1000);
		printf("%ld %d is sleeping\n", time_passed(philinfo->state->time_since_start, &timer), philinfo->philo_number);
		dead = 1;
	}
	printf("%ld %d has died\n", time_passed(philinfo->state->time_since_start, &timer), philinfo->philo_number);
	return (NULL);
}

int	main(int ac, char **av)
{
	t_state			*state;
	pthread_mutex_t	*forks;
	pthread_t		*philos;

	if (ac != 5 && ac != 6)
	{
		printf("arg error\nnumber_of_philosophers | time_to_die | time_to_eat | time_to_sleep | [number_of_time_each_philosopher_must_eat]\n");
		exit(EXIT_FAILURE);
	}
	state = init_state(av);
	forks = init_mutex(state);
	philos = init_threads(state, forks);
	// printf("nbr : %d | time_to_die : %d | time_to_eat : %d | time_to_sleep : %d | nbr_of_time_eat : %d \n", \
	// 	state->number_of_philosophers, state->time_to_die, state->time_to_eat, state->time_to_sleep, state->number_of_time_each_philosopher_must_eat);
	join_threads(state, philos);
	destroy_mutex(state, forks);
	exit(EXIT_SUCCESS);
}
