/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_does.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 18:46:39 by lpellier          #+#    #+#             */
/*   Updated: 2021/11/30 16:03:14 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	gettimeofday(&philo->time_since_last_meal, NULL);
	output(philo, "is eating");
	better_usleep(philo->args.time_to_eat);
	pthread_mutex_unlock(philo->adjacent_forks[1]);
	philo->holding_forks--;
	pthread_mutex_unlock(philo->adjacent_forks[0]);
	philo->holding_forks--;
	pthread_mutex_lock(philo->args.output_lock);
	philo->meals_eaten++;
	pthread_mutex_unlock(philo->args.output_lock);
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
	else if (philo->does == FORK)
		philo_takes_forks(philo);
	else if (philo->does == EAT)
		philo_eats(philo);
	else if (philo->does == SLEEP)
		philo_sleeps(philo);
}
