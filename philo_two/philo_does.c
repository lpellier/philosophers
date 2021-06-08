/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_does.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 15:30:49 by lpellier          #+#    #+#             */
/*   Updated: 2021/06/08 17:33:00 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

void	philo_thinks(t_philo *philo)
{
	output(philo, "is thinking");
	philo->does = FORK;
}

void	philo_takes_forks(t_philo *philo)
{
	sem_wait(philo->info->forks);
	output(philo, "has taken a fork");
	sem_wait(philo->info->forks);
	output(philo, "has taken a fork");
	philo->does = EAT;
}

void	philo_eats(t_philo *philo)
{
	pthread_t	timer;

	gettimeofday(&philo->time_since_last_meal, NULL);
	output(philo, "is eating");
	better_usleep(philo->info->time_to_eat);
	sem_post(philo->info->forks);
	sem_post(philo->info->forks);
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
