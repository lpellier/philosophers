/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 15:32:11 by lpellier          #+#    #+#             */
/*   Updated: 2021/06/15 14:31:19 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

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
	sem_t	*lock;

	lock = sem_open("lock", O_RDWR);
	if (!philo->is_alive)
		return ;
	sem_wait(lock);
	printf("\x1b[36m%5ld \033[31m%d \x1b[36m%s\n", \
		time_passed(&philo->time_since_last_meal), philo->philo_number, msg);
	sem_post(lock);
}

void	*check_time(void *arg)
{
	t_philo			*philo;
	sem_t			*lock;

	philo = arg;
	while (philo->does != EAT)
	{
		if (time_passed(&philo->time_since_last_meal) > \
			philo->info->time_to_die)
		{
			if (!philo->is_alive)
				return (NULL);
			output(philo, "has died");
			philo->is_alive = false;
			lock = sem_open("lock", O_RDWR);
			sem_wait(lock);
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
