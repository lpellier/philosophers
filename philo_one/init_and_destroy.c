/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_destroy.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 15:01:05 by lpellier          #+#    #+#             */
/*   Updated: 2021/06/04 15:51:49 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philinfo	*create_philinfo(t_state *state, pthread_mutex_t *forks, int index)
{
	t_philinfo		*philinfo;

	if (ft_calloc((void **)&philinfo, 1, sizeof(t_philinfo)))
		return (NULL);
	if (ft_calloc((void **)&philinfo->adjacent_forks, 2, sizeof(pthread_mutex_t)))
		return (NULL);
	if (index > 0)
		philinfo->adjacent_forks[0] = forks[index - 1];
	else
		philinfo->adjacent_forks[0] = forks[state->number_of_philosophers - 1];
	if (index < state->number_of_philosophers)
		philinfo->adjacent_forks[1] = forks[index + 1];
	else
		philinfo->adjacent_forks[1] = forks[0];
	philinfo->philo_number = index + 1;
	philinfo->state = state;
	return (philinfo);
}

pthread_t	*init_threads(t_state *state, pthread_mutex_t *forks)
{
	int				i;
	pthread_t		*threads;
	t_philinfo		**philinfo;

	i = 0;
	if (ft_calloc((void **)&threads, state->number_of_philosophers, sizeof(pthread_t)))
		return (NULL);
	if (ft_calloc((void **)&philinfo, state->number_of_philosophers, sizeof(t_philinfo *)))
		return (NULL);
	while (i < state->number_of_philosophers)
	{
		philinfo[i] = create_philinfo(state, forks, i);
		if (!philinfo[i])
			return (NULL);
		pthread_create(&threads[i], NULL, &philo_routine, philinfo[i]);
		i++;
	}
	// need to free philinfo * at some point
	// but not here - in the calling function
	return (threads);
}

pthread_mutex_t	*init_mutex(t_state *state)
{
	pthread_mutex_t *mutex;
	int	i;

	i = 0;
	if (ft_calloc((void **)&mutex, state->number_of_philosophers, sizeof(pthread_mutex_t)))
		return (NULL);
	while (i < state->number_of_philosophers)
	{
		pthread_mutex_init(&mutex[i], NULL);
		i++;
	}
	return (mutex);
}

void	join_threads(t_state *state, pthread_t *threads)
{
	int		i;

	i = 0;
	while (i < state->number_of_philosophers)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

void	destroy_mutex(t_state *state, pthread_mutex_t *mutex)
{
	int		i;

	i = 0;
	while (i < state->number_of_philosophers)
	{
		pthread_mutex_destroy(&mutex[i]);
		i++;
	}
}

t_state		*init_state(char **av)
{
	t_state *state;

	if ((ft_calloc((void **)&state, 1, sizeof(t_state))))
		return (NULL);
	state->number_of_philosophers = ft_atoi(av[1]);
	state->time_to_die = ft_atoi(av[2]);
	state->time_to_eat = ft_atoi(av[3]);
	state->time_to_sleep = ft_atoi(av[4]);
	state->number_of_time_each_philosopher_must_eat = -1;
	gettimeofday(&state->time_since_start, NULL);
	if (av[5])
		state->number_of_time_each_philosopher_must_eat = ft_atoi(av[5]);
	return (state);
}
