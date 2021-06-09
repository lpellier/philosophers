/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 16:26:35 by lpellier          #+#    #+#             */
/*   Updated: 2021/06/09 15:21:43 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

t_state	*init_state(char **av)
{
	t_state	*state;
	t_info	*info;

	if ((ft_calloc((void **)&state, 1, sizeof(t_state))))
		return (NULL);
	if ((ft_calloc((void **)&info, 1, sizeof(t_info))))
		return (NULL);
	info->number_of_philosophers = ft_atoi(av[1]);
	info->time_to_die = ft_atoi(av[2]);
	info->time_to_eat = ft_atoi(av[3]);
	info->time_to_sleep = ft_atoi(av[4]);
	info->meal_goal = -1;
	gettimeofday(&info->time_since_start, NULL);
	if (av[5])
		info->meal_goal = ft_atoi(av[5]);
	state->info = info;
	init_forks(state);
	state->philos = init_philos(info);
	return (state);
}

int	philo_routine(t_philo *philo)
{
	pthread_t	timer;
	sem_t		*forks;

	forks = sem_open("forks", O_RDWR);
	gettimeofday(&philo->time_since_last_meal, NULL);
	pthread_create(&timer, NULL, &check_time, (void *)philo);
	pthread_detach(timer);
	while (philo->is_alive)
	{
		philo_does(philo, forks);
		if (philo->info->meal_goal != -1 && \
			philo->number_of_meals >= philo->info->meal_goal)
			break ;
	}
	if (philo->is_alive && philo->info->meal_goal != -1)
		output(philo, "is done");
	return (0);
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

int	main(int ac, char **av)
{
	t_state			*state;

	if ((ac != 5 && ac != 6) || error_in_args(av))
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
	exit(EXIT_SUCCESS);
}
