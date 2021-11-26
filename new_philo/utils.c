/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 17:40:21 by lpellier          #+#    #+#             */
/*   Updated: 2021/11/26 19:03:40 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	better_usleep(int time)
{
	long	desired_sleep;

	desired_sleep = time_passed(NULL) + time;
	while (time_passed(NULL) < desired_sleep)
		usleep(500);
}

int	error_in_args(char **av)
{
	int	i;
	int	j;

	i = 1;
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (!(av[i][j] >= 48 && av[i][j] <= 57))
				return (EXIT_FAILURE);
			j++;
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

int	ft_atoi(const char *str)
{
	int	neg;
	int	i;
	int	num;

	i = 0;
	neg = 1;
	num = 0;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t' || \
		str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			neg *= -1;
		i++;
	}
	while (str[i] >= 48 && str[i] <= 57)
	{
		num = num * 10 + (str[i] - 48);
		i++;
	}
	return (num * neg);
}

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
