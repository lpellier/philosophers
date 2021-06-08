/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 16:26:35 by lpellier          #+#    #+#             */
/*   Updated: 2021/06/08 12:55:05 by lpellier         ###   ########.fr       */
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
	while (philo->does != EAT)
	{
		if (time_passed(&philo->time_since_last_meal) > philo->info->time_to_die)
		{
			philo->is_dead = TRUE;
			pthread_mutex_lock(&philo->info->lock);
			printf("\x1b[36m%5ld \033[31m%d \x1b[36mhas died\n", \
				time_passed(&philo->time_since_last_meal), philo->philo_number);
			return (NULL);
		}
	}
	return (NULL);
}

void	better_usleep(int time)
{
	long	desired_sleep;

	desired_sleep = time_passed(NULL) + time;
	while (time_passed(NULL) <= desired_sleep)
		usleep(1000);
}

void	philo_thinks(t_philo *philo)
{
	printf("\x1b[36m%5ld \033[31m%d \x1b[36mis thinking\n", \
		time_passed(&philo->time_since_last_meal), philo->philo_number);
	philo->does = FORK;
}

void	philo_takes_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->adjacent_forks[0]);
	printf("\x1b[36m%5ld \033[31m%d \x1b[36mhas taken a fork\x1b[0m\n", \
		time_passed(&philo->time_since_last_meal), philo->philo_number);
	pthread_mutex_lock(philo->adjacent_forks[1]);
	printf("\x1b[36m%5ld \033[31m%d \x1b[36mhas taken a fork\x1b[0m\n", \
		time_passed(&philo->time_since_last_meal), philo->philo_number);
	philo->does = EAT;
}

void	philo_eats(t_philo *philo)
{
	pthread_t	timer;

	gettimeofday(&philo->time_since_last_meal, NULL);
	printf("\x1b[36m%5ld \033[31m%d \x1b[36mis eating\x1b[0m\n", \
		time_passed(&philo->time_since_last_meal), philo->philo_number);
	better_usleep(philo->info->time_to_eat);
	pthread_mutex_unlock(philo->adjacent_forks[0]);
	pthread_mutex_unlock(philo->adjacent_forks[1]);
	philo->number_of_meals++;
	if (philo->info->meal_goal != -1 && \
		philo->number_of_meals >= philo->info->meal_goal)
		return ;
	pthread_create(&timer, NULL, &check_time, philo);
	pthread_detach(timer);
	philo->does = SLEEP;
}

void	philo_sleeps(t_philo *philo)
{
	printf("\x1b[36m%5ld \033[31m%d \x1b[36mis sleeping\n", \
		time_passed(&philo->time_since_last_meal), philo->philo_number);
	better_usleep(philo->info->time_to_sleep);
	philo->does = THINK;
}

void	philo_does(t_philo *philo)
{
	if (!philo->info->everyone_is_alive)
		pthread_mutex_lock(&philo->info->lock);
	if (philo->does == THINK)
		philo_thinks(philo);
	else if (philo->does == FORK)
		philo_takes_forks(philo);
	else if (philo->does == EAT)
		philo_eats(philo);
	else if (philo->does == SLEEP)
		philo_sleeps(philo);
}

void	*philo_routine(void *arg)
{
	t_philo		*philo;
	pthread_t	timer;

	philo = arg;
	gettimeofday(&philo->time_since_last_meal, NULL);
	pthread_create(&timer, NULL, &check_time, (void *)philo);
	pthread_detach(timer);
	while (!philo->is_dead)
	{
		philo_does(philo);
		if ((philo->info->meal_goal != -1 && philo->number_of_meals >= philo->info->meal_goal) || \
			!philo->info->everyone_is_alive)
			break ;
	}
	if (philo->is_dead)
		philo->info->everyone_is_alive = FALSE;
	if (!philo->is_dead && philo->info->meal_goal != -1 && philo->info->everyone_is_alive)
	{
		printf("\x1b[36m%5ld \033[31m%d \x1b[36mis done\n", \
		time_passed(&philo->time_since_last_meal), philo->philo_number);
	}
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
