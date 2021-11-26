/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_does.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 18:46:39 by lpellier          #+#    #+#             */
/*   Updated: 2021/11/26 19:03:16 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	time_passed(struct timeval *ref)
{
	struct timeval	timer;

	gettimeofday(&timer, NULL);
	if (ref)
		return ((double)(timer.tv_sec - ref->tv_sec) *1000 + \
			(long)(timer.tv_usec - ref->tv_usec) *0.001);
	else
		return ((double)(timer.tv_sec * 1000) + (long)(timer.tv_usec * 0.001));
}

void	output(t_philo *philo, char *msg)
{
	int	everyone_alive;

	pthread_mutex_lock(philo->args.output_lock);
	everyone_alive = *(philo->args.everyone_is_alive);
	pthread_mutex_unlock(philo->args.output_lock);
	if (!everyone_alive)
		return ;
	pthread_mutex_lock(philo->args.output_lock);
	printf("\x1b[36m%5ld \033[31m%d \x1b[36m%s\n", \
		time_passed(&philo->time_since_last_meal), philo->philo_index, msg);
	pthread_mutex_unlock(philo->args.output_lock);
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
	philo->holding_forks++;
	pthread_mutex_lock(philo->adjacent_forks[1]);
	output(philo, "has taken a fork");
	philo->holding_forks++;
	philo->does = EAT;
}

void	philo_eats(t_philo *philo)
{
	pthread_mutex_lock(philo->args.output_lock);
	gettimeofday(&philo->time_since_last_meal, NULL);
	pthread_mutex_unlock(philo->args.output_lock);
	output(philo, "is eating");
	better_usleep(philo->args.time_to_eat);
	pthread_mutex_unlock(philo->adjacent_forks[0]);
	philo->holding_forks++;
	pthread_mutex_unlock(philo->adjacent_forks[1]);
	philo->holding_forks++;
	philo->meals_eaten++;
	philo->does = SLEEP;
}

void	philo_sleeps(t_philo *philo)
{
	output(philo, "is sleeping");
	better_usleep(philo->args.time_to_sleep);
	philo->does = THINK;
}

void	philo_does(t_philo *philo)
{
	if (philo->does == THINK)
		philo_thinks(philo);
	else if  (philo->does == FORK)
		philo_takes_forks(philo);
	else if (philo->does == EAT)
		philo_eats(philo);
	else if (philo->does == SLEEP)
		philo_sleeps(philo);
}
