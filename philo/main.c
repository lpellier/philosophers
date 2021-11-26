/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 16:26:35 by lpellier          #+#    #+#             */
/*   Updated: 2021/11/26 17:10:11 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	info->everyone_is_alive = true;
	gettimeofday(&info->time_since_start, NULL);
	if (av[5])
		info->meal_goal = ft_atoi(av[5]);
	state->info = info;
	state->forks = init_forks(info);
	state->philos = init_philos(info, state->forks);
	return (state);
}

void	*philo_routine(void *arg)
{
	t_philo		*philo;
	pthread_t	timer;

	philo = arg;
	gettimeofday(&philo->time_since_last_meal, NULL);
	pthread_create(&timer, NULL, &check_time, (void *)philo);
	pthread_detach(timer);
	pthread_mutex_lock(&philo->info->var_lock);
	while (philo->info->everyone_is_alive)
	{
		pthread_mutex_unlock(&philo->info->var_lock);
		philo_does(philo);
		if (philo->meal_goal != -1 && \
			philo->number_of_meals >= philo->meal_goal)
			break ;
		pthread_mutex_lock(&philo->info->var_lock);
	}
	if (philo->holding >= 1)
		pthread_mutex_unlock(philo->adjacent_forks[0]);
	if (philo->holding >= 2)
		pthread_mutex_unlock(philo->adjacent_forks[1]);
	philo->holding = 0;
	if (philo->info->everyone_is_alive && philo->meal_goal != -1)
	{
		pthread_mutex_unlock(&philo->info->var_lock);
		output(philo, "is done");
		return (NULL);
	}
	pthread_mutex_unlock(&philo->info->var_lock);
	return (NULL);
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
	secure_free(state->forks);
	exit(EXIT_SUCCESS);
}
