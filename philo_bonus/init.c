/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 17:42:44 by lpellier          #+#    #+#             */
/*   Updated: 2021/12/02 14:29:34 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

sem_t	*init_forks(t_args *args)
{
	sem_t	*forks;

	sem_unlink("forks");
	sem_unlink("output_lock");
	sem_unlink("stop");
	forks = sem_open("forks", O_CREAT, 00644, args->nbr_of_philos);
	args->stop_program = sem_open("stop", O_CREAT, 00644, 1);
	args->output_lock = sem_open("output_lock", O_CREAT, 00644, 1);
	return (forks);
}

t_philo	create_philo(t_args args, int index)
{
	t_philo	philo;

	philo.philo_index = index + 1;
	philo.does = SLEEP;
	if (philo.philo_index % 2 == 0)
		philo.does = FORK;
	philo.holding_forks = 0;
	philo.args.nbr_of_philos = args.nbr_of_philos;
	philo.args.time_to_die = args.time_to_die;
	philo.args.time_to_eat = args.time_to_eat;
	philo.args.time_to_sleep = args.time_to_sleep;
	philo.args.meal_goal = args.meal_goal;
	philo.args.everyone_is_alive = args.everyone_is_alive;
	philo.args.output_lock = args.output_lock;
	philo.meals_eaten = 0;
	return (philo);
}

void	*create_process(t_philo *philo)
{
	int		status;
	pid_t	cpid;

	cpid = fork();
	status = 1;
	philo->cpid = cpid;
	if (cpid == -1)
		return (NULL);
	else if (cpid == 0)
		exit(philo_routine(philo));
	else
		return (NULL);
}

t_philo	*init_philos(t_args args)
{
	t_philo	*philos;
	int		index;

	index = 0;
	
	philos = malloc(sizeof(t_philo) * args.nbr_of_philos);
	if (!philos)
		return (NULL);
	while (index < args.nbr_of_philos)
	{
		philos[index] = create_philo(args, index);
		if (create_process(&philos[index]))
			return (NULL);
		index++;
	}
	return (philos);
}
