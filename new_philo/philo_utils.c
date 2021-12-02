/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 16:01:21 by lpellier          #+#    #+#             */
/*   Updated: 2021/12/02 10:33:56 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	fill_args(char **av, t_args *args)
{
	args->everyone_is_alive = malloc(sizeof(int));
	if (!args->everyone_is_alive)
		return (1);
	*(args->everyone_is_alive) = 1;
	args->output_lock = malloc(sizeof(pthread_mutex_t));
	if (!args->output_lock)
	{
		secure_free((void **)&args->everyone_is_alive);
		return (1);
	}
	pthread_mutex_init(args->output_lock, NULL);
	args->nbr_of_philos = ft_atoi(av[1]);
	args->time_to_die = ft_atoi(av[2]);
	args->time_to_eat = ft_atoi(av[3]);
	args->time_to_sleep = ft_atoi(av[4]);
	args->meal_goal = -1;
	if (av[5])
		args->meal_goal = ft_atoi(av[5]);
	return (0);
}

void	better_usleep(int time)
{
	long	desired_sleep;

	desired_sleep = time_passed(NULL) + time;
	while (time_passed(NULL) < desired_sleep)
		usleep(500);
}

long	time_passed(struct timeval *ref)
{
	struct timeval	timer;

	gettimeofday(&timer, NULL);
	if (ref)
		return ((double)(timer.tv_sec - ref->tv_sec) * 1000 + \
			(long)(timer.tv_usec - ref->tv_usec) * 0.001);
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

int	error_in_args(char **av)
{
	int	i;
	int	j;

	i = 1;
	if (ft_atoi(av[i]) <= 1)
		return (EXIT_FAILURE);
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (!(av[i][j] >= 48 && av[i][j] <= 57))
				return (EXIT_FAILURE);
			j++;
		}
		if (ft_strlen(av[i]) > INTMAX_LEN + 1 || ft_atoi(av[i]) > INT_MAX)
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}
