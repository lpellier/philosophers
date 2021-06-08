/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 16:26:35 by lpellier          #+#    #+#             */
/*   Updated: 2021/06/08 15:19:38 by lpellier         ###   ########.fr       */
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
		return ((double)((timer.tv_sec - ref->tv_sec) * 1000) + \
			((long)(timer.tv_usec - ref->tv_usec) * 0.001));
	else
		return ((double)(timer.tv_sec * 1000) + (long)(timer.tv_usec * 0.001));
}

void	output(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->info->output_lock);
	if (!philo->info->everyone_is_alive)
	{
		pthread_mutex_unlock(&philo->info->output_lock);
		return ;
	}
	printf("\x1b[36m%5ld \033[31m%d \x1b[36m%s\n", \
		time_passed(&philo->time_since_last_meal), philo->philo_number, msg);
	pthread_mutex_unlock(&philo->info->output_lock);
}

void	*check_time(void *arg)
{
	t_philo			*philo;

	philo = arg;
	while (philo->does != EAT)
	{
		if (time_passed(&philo->time_since_last_meal) > philo->info->time_to_die)
		{
			if (!philo->info->everyone_is_alive)
				return (NULL);
			output(philo, "has died");
			philo->info->everyone_is_alive = FALSE;
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
		usleep(500);
}

void	philo_thinks(t_philo *philo)
{
	output(philo, "is thinking");
	philo->does = FORK;
}

void	philo_takes_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->adjacent_forks[0]);
	output(philo, "has taken a fork");
	pthread_mutex_lock(philo->adjacent_forks[1]);
	output(philo, "has taken a fork");
	philo->does = EAT;
}

void	philo_eats(t_philo *philo)
{
	pthread_t	timer;

	gettimeofday(&philo->time_since_last_meal, NULL);
	output(philo, "is eating");
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
	output(philo, "is sleeping");
	better_usleep(philo->info->time_to_sleep);
	philo->does = THINK;
}

void	philo_does(t_philo *philo)
{
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
	while (philo->info->everyone_is_alive)
	{
		philo_does(philo);
		if (philo->info->meal_goal != -1 && \
			philo->number_of_meals >= philo->info->meal_goal)
			break ;
	}
	if (philo->info->everyone_is_alive && philo->info->meal_goal != -1)
		output(philo, "is done");
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
