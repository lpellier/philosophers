/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_destroy.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 15:01:05 by lpellier          #+#    #+#             */
/*   Updated: 2021/06/10 11:19:14 by lpellier         ###   ########.fr       */
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

void	kill_all_processes(t_process *process)
{
	int		i;

	i = 0;
	while (i < process->nbr_of_philos)
	{
		kill(process->philos[i].cpid, SIGTERM);
		i++;
	}
}

void	*philo_process(void *arg)
{
	int			status;
	t_process	*process;
	pid_t		cpid;

	process = arg;
	cpid = fork();
	status = 1;
	process->philos[process->philo_index].cpid = cpid;
	if (cpid == -1)
		return (NULL);
	else if (cpid == 0)
		exit(philo_routine(&process->philos[process->philo_index]));
	else
	{
		waitpid(cpid, &status, 0);
		if (status)
			kill_all_processes(process);
		return (NULL);
	}
}

t_philo	*init_philos(t_info *info)
{
	int			i;
	t_philo		*philos;
	t_process	*processes;

	if (ft_calloc((void **)&philos, info->number_of_philosophers, \
		sizeof(t_philo)))
		return (NULL);
	if (ft_calloc((void **)&processes, info->number_of_philosophers, \
		sizeof(t_process)))
		return (NULL);
	i = 0;
	while (i < info->number_of_philosophers)
	{
		philos[i] = create_philo(info, i);
		processes[i].philo_index = i;
		processes[i].nbr_of_philos = info->number_of_philosophers;
		i++;
	}
	i = 0;
	while (i < info->number_of_philosophers)
	{
		processes[i].philos = philos;
		pthread_create(&philos[i].thread, NULL, philo_process, &processes[i]);
		i++;
	}
	return (philos);
}

void	init_forks(t_state *state)
{
	int				i;

	i = 0;
	sem_unlink("forks");
	state->forks = sem_open("forks", O_CREAT, 00644, \
		state->info->number_of_philosophers);
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
	sem_close(state->forks);
	sem_unlink("forks");
}
