/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 11:33:08 by lpellier          #+#    #+#             */
/*   Updated: 2021/10/04 11:33:29 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	kill_all_processes(t_process *process)
{
	int		i;

	i = 0;
	while (i < process->nbr_of_philos)
	{
		kill(process->philos[i].cpid, SIGKILL);
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
