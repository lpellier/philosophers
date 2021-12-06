/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_does.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 18:46:39 by lpellier          #+#    #+#             */
/*   Updated: 2021/12/02 11:42:41 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	philo_thinks(t_philo *philo)
{
	output(philo, "is thinking");
	philo->does = FORK;
}

void	philo_takes_forks(t_philo *philo, sem_t *forks)
{
	sem_wait(forks);
	output(philo, "has taken a fork");
	philo->holding_forks++;
	sem_wait(forks);
	output(philo, "has taken a fork");
	philo->holding_forks++;
	philo->does = EAT;
}

void	philo_eats(t_philo *philo, sem_t *forks)
{
	gettimeofday(&philo->time_since_last_meal, NULL);
	output(philo, "is eating");
	better_usleep(philo->args.time_to_eat);
	sem_post(forks);
	philo->holding_forks--;
	sem_post(forks);
	philo->holding_forks--;
	sem_wait(philo->args.output_lock);
	philo->meals_eaten++;
	sem_post(philo->args.output_lock);
	philo->does = SLEEP;
}

void	philo_sleeps(t_philo *philo)
{
	output(philo, "is sleeping");
	better_usleep(philo->args.time_to_sleep);
	philo->does = THINK;
}

void	philo_does(t_philo *philo, sem_t *forks)
{
	if (philo->does == THINK)
		philo_thinks(philo);
	else if (philo->does == FORK)
		philo_takes_forks(philo, forks);
	else if (philo->does == EAT)
		philo_eats(philo, forks);
	else if (philo->does == SLEEP)
		philo_sleeps(philo);
}
