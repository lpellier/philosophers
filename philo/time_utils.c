/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 15:32:11 by lpellier          #+#    #+#             */
/*   Updated: 2021/11/26 17:09:11 by lpellier         ###   ########.fr       */
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

	pthread_mutex_lock(&philo->info->var_lock);
	if (!philo->info->everyone_is_alive)
	{
		pthread_mutex_unlock(&philo->info->var_lock);
		return ;
	}
	pthread_mutex_unlock(&philo->info->var_lock);
	pthread_mutex_lock(&philo->info->output_lock);
	printf("\x1b[36m%5ld \033[31m%d \x1b[36m%s\n", \
		time_passed(&philo->time_since_last_meal), philo->philo_number, msg);
	pthread_mutex_unlock(&philo->info->output_lock);
}

void	*check_time(void *arg)
{
	t_philo			*philo;

	philo = arg;
	while (1)
	{
		pthread_mutex_lock(&philo->info->var_lock);
		if (!philo->info->everyone_is_alive)
		{
			if (philo->holding >= 1)
				pthread_mutex_unlock(philo->adjacent_forks[0]);
			if (philo->holding >= 2)
				pthread_mutex_unlock(philo->adjacent_forks[1]);
			philo->holding = 0;
			pthread_mutex_unlock(&philo->info->var_lock);
			return (NULL);
		}
		if (time_passed(&philo->time_since_last_meal) > philo->time_to_die)
		{
			if (!philo->info->everyone_is_alive)
			{
				pthread_mutex_unlock(&philo->info->var_lock);
				return (NULL);
			}
			pthread_mutex_unlock(&philo->info->var_lock);
			output(philo, "has died");
			pthread_mutex_lock(&philo->info->var_lock);
			philo->info->everyone_is_alive = false;
			pthread_mutex_unlock(&philo->info->var_lock);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->info->var_lock);
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
