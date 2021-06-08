/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_destroy.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 15:01:05 by lpellier          #+#    #+#             */
/*   Updated: 2021/06/08 17:43:12 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

t_philo	create_philo(t_info *info, int index)
{
	t_philo		philo;

	philo.philo_number = index + 1;
	philo.info = info;
	philo.does = SLEEP;
	philo.number_of_meals = 0;
	philo.is_alive = true;
	if (philo.philo_number % 2 == 0)
		philo.does = FORK;
	return (philo);
}

void	*philo_process(void *arg)
{
	int		status;
	t_philo *philo;
	pid_t	cpid;

	philo = arg;
	cpid = fork();
	philo->cpid = cpid;
	if (cpid == -1)
		return (NULL);
	else if (cpid == 0)
		exit(philo_routine(philo));
	else
	{
		waitpid(cpid, &status, 0);
		return (NULL);
	}
}

t_philo	*init_philos(t_info *info)
{
	int			i;
	t_philo		*philos;

	i = 0;
	if (ft_calloc((void **)&philos, info->number_of_philosophers, \
		sizeof(t_philo)))
		return (NULL);
	while (i < info->number_of_philosophers)
	{
		philos[i] = create_philo(info, i);
		pthread_create(&philos[i].thread, NULL, philo_process, &philos[i]);
		i++;
	}
	return (philos);
}

sem_t	*init_forks(t_info *info)
{
	int				i;
	sem_t			*forks;

	i = 0;
	if (ft_calloc((void **)&forks, 1, sizeof(sem_t)))
		return (NULL);
	sem_unlink("forks");
	forks = sem_open("forks", O_CREAT, 00644, info->number_of_philosophers);
	return (forks);
}

void	join_philos(t_state *state)
{
	int		i;

	i = 0;
	while (i < state->info->number_of_philosophers)
	{
		pthread_join(state->philos[i].thread, NULL);
		kill(state->philos[i].cpid, SIGINT);
		i++;
	}
}

void	destroy_forks(t_state *state)
{
	int		i;

	i = 0;
	sem_close(state->info->forks);
	sem_unlink("forks");
}
