/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 16:26:35 by lpellier          #+#    #+#             */
/*   Updated: 2021/06/05 18:05:06 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	secure_free(void *ptr)
{
	if (ptr)
		free(ptr);
	ptr = NULL;
}

long	time_passed(struct timeval *ref)
{
	struct timeval	timer;

	gettimeofday(&timer, NULL);
	if (ref)
		return (((timer.tv_sec - ref->tv_sec) * 1000) + \
			((timer.tv_usec - ref->tv_usec) / 1000));
	else
		return ((timer.tv_sec * 1000) + (timer.tv_usec / 1000));
}

void	*check_time(void *arg)
{
	t_philo			*philo;

	philo = arg;
	while (!philo->is_eating)
	{
		if (time_passed(&philo->time_since_last_meal) > philo->info->time_to_die)
		{
			philo->is_dead = TRUE;
			return (NULL);
		}
	}
	return (NULL);
}

void	better_usleep(int time)
{
	long	desired_sleep;

	desired_sleep = time_passed(NULL) + time;
	while (time_passed(NULL) < desired_sleep)
		usleep(1000);
}

void	philo_does(t_philo *philo, int action)
{
	if (action == THINK)
		printf("\x1b[36m%5ld \033[31m%d \x1b[36mis thinking\n", \
			time_passed(&philo->time_since_last_meal), philo->philo_number);
	else if (action == FORK)
		printf("\x1b[36m%5ld \033[31m%d \x1b[36mhas taken a fork\x1b[0m\n", \
			time_passed(&philo->time_since_last_meal), philo->philo_number);
	else if (action == EAT)
	{
		printf("\x1b[36m%5ld \033[31m%d \x1b[36mis eating\n", \
			time_passed(&philo->time_since_last_meal), philo->philo_number);
		better_usleep(philo->info->time_to_eat);
		gettimeofday(&philo->time_since_last_meal, NULL);
	}
	else if (action == SLEEP)
	{
		printf("\x1b[36m%5ld \033[31m%d \x1b[36mis sleeping\n", \
			time_passed(&philo->time_since_last_meal), philo->philo_number);
		better_usleep(philo->info->time_to_sleep);
	}
	else if (action == DIE)
		printf("\x1b[36m%5ld \033[31m%d \x1b[36mhas died\n", \
			time_passed(&philo->time_since_last_meal), philo->philo_number);
}

void	*philo_routine(void *arg)
{
	t_philo		*philo;
	pthread_t	timer;

	philo = arg;
	philo->is_dead = FALSE;
	philo->is_eating = FALSE;
	gettimeofday(&philo->time_since_last_meal, NULL);
	while (!philo->is_dead)
	{
		philo_does(philo, THINK);
		pthread_create(&timer, NULL, &check_time, (void *)philo);
		pthread_detach(timer);
		pthread_mutex_lock(philo->adjacent_forks[0]);
		if (philo->is_dead)
		{
			pthread_mutex_unlock(philo->adjacent_forks[0]);
			break;
		}
		philo_does(philo, FORK);
		pthread_mutex_lock(philo->adjacent_forks[1]);
		philo_does(philo, FORK);
		philo->is_eating = TRUE;
		philo_does(philo, EAT);
		pthread_mutex_unlock(philo->adjacent_forks[0]);
		pthread_mutex_unlock(philo->adjacent_forks[1]);
		philo->is_eating = FALSE;
		if (!philo->is_dead)
			philo_does(philo, SLEEP);
	}
	philo_does(philo, DIE);
	return (NULL);
}

int	main(int ac, char **av)
{
	t_state			*state;

	if (ac != 5 && ac != 6)
	{
		printf("arg error\nnumber_of_philosophers | ");
		printf("time_to_die | time_to_eat | time_to_sleep | ");
		printf("[number_of_time_each_philosopher_must_eat]\n");
		exit(EXIT_FAILURE);
	}
	state = init_state(av);
	join_philos(state);
	destroy_forks(state);
	secure_free(state->info);
	secure_free(state->philos);
	secure_free(state->forks);
	exit(EXIT_SUCCESS);
}
