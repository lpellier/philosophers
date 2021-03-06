/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_does.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 15:30:49 by lpellier          #+#    #+#             */
/*   Updated: 2021/11/26 17:05:08 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_thinks(t_philo *philo)
{
	output(philo, "is thinking");
	pthread_mutex_lock(&philo->info->var_lock);
	philo->does = FORK;
	pthread_mutex_unlock(&philo->info->var_lock);
}

void	philo_takes_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->adjacent_forks[0]);
	output(philo, "has taken a fork");
	pthread_mutex_lock(&philo->info->var_lock);
	philo->holding++;
	pthread_mutex_unlock(&philo->info->var_lock);
	pthread_mutex_lock(philo->adjacent_forks[1]);
	output(philo, "has taken a fork");
	pthread_mutex_lock(&philo->info->var_lock);
	philo->holding++;
	philo->does = EAT;
	pthread_mutex_unlock(&philo->info->var_lock);
}

void	philo_eats(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->var_lock);
	gettimeofday(&philo->time_since_last_meal, NULL);
	pthread_mutex_unlock(&philo->info->var_lock);
	output(philo, "is eating");
	better_usleep(philo->time_to_eat);
	pthread_mutex_unlock(philo->adjacent_forks[0]);
	pthread_mutex_lock(&philo->info->var_lock);
	philo->holding++;
	pthread_mutex_unlock(&philo->info->var_lock);
	pthread_mutex_unlock(philo->adjacent_forks[1]);
	pthread_mutex_lock(&philo->info->var_lock);
	philo->holding++;
	philo->number_of_meals++;
	philo->does = SLEEP;
	pthread_mutex_unlock(&philo->info->var_lock);
}

void	philo_sleeps(t_philo *philo)
{
	output(philo, "is sleeping");
	better_usleep(philo->time_to_sleep);
	pthread_mutex_lock(&philo->info->var_lock);
	philo->does = THINK;
	pthread_mutex_unlock(&philo->info->var_lock);
}

void	philo_does(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->var_lock);
	if (philo->does == THINK)
	{
		pthread_mutex_unlock(&philo->info->var_lock);
		philo_thinks(philo);
	}
	else if (philo->does == FORK)
	{
		pthread_mutex_unlock(&philo->info->var_lock);
		philo_takes_forks(philo);
	}
	else if (philo->does == EAT)
	{
		pthread_mutex_unlock(&philo->info->var_lock);
		philo_eats(philo);
	}
	else if (philo->does == SLEEP)
	{
		pthread_mutex_unlock(&philo->info->var_lock);
		philo_sleeps(philo);
	}
}
