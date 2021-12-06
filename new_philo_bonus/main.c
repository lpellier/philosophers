/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 17:19:49 by lpellier          #+#    #+#             */
/*   Updated: 2021/12/02 14:30:33 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	check_philo_death(t_philo *philo, int everyone_alive)
{
	if (time_passed(&philo->time_since_last_meal) > philo->args.time_to_die)
	{
		if (!everyone_alive || philo->meals_eaten == philo->args.meal_goal)
		{
			sem_post(philo->args.output_lock);
			return ;
		}
		sem_post(philo->args.output_lock);
		output(philo, "has died");
		sem_wait(philo->args.output_lock);
		*(philo->args.everyone_is_alive) = 0;
		sem_post(philo->args.output_lock);
	}
}

void	*check_time(void *arg)
{
	t_philo	*philo;
	int		everyone_alive;

	philo = arg;
	everyone_alive = 1;
	while (everyone_alive)
	{
		sem_wait(philo->args.output_lock);
		if (!everyone_alive || philo->meals_eaten == philo->args.meal_goal)
		{
			sem_post(philo->args.output_lock);
			return (NULL);
		}
		check_philo_death(philo, everyone_alive);
		everyone_alive = *(philo->args.everyone_is_alive);
		sem_post(philo->args.output_lock);
	}
	return (NULL);
}

void	loop_routine(int *everyone_alive, t_philo *philo, sem_t *forks)
{
	while (*everyone_alive)
	{
		philo_does(philo, forks);
		if (philo->args.meal_goal != -1 && \
			philo->meals_eaten >= philo->args.meal_goal)
			break ;
		sem_wait(philo->args.output_lock);
		*everyone_alive = *(philo->args.everyone_is_alive);
		sem_post(philo->args.output_lock);
	}
}

int	philo_routine(void *arg)
{
	pthread_t	timer;
	t_philo		*philo;
	sem_t		*forks;
	int			everyone_alive;

	philo = (t_philo *)arg;
	forks = sem_open("forks", O_RDWR);
	gettimeofday(&philo->time_since_last_meal, NULL);
	sem_wait(philo->args.output_lock);
	pthread_create(&timer, NULL, &check_time, (void *)philo);
	sem_post(philo->args.output_lock);
	everyone_alive = 1;
	loop_routine(&everyone_alive, philo, forks);
	if (philo->holding_forks >= 1)
		sem_post(forks);
	if (philo->holding_forks >= 2)
		sem_post(forks);
	philo->holding_forks = 0;
	if (everyone_alive && philo->args.meal_goal != -1)
		output(philo, "is done");
	pthread_join(timer, NULL);
	// destroy_forks(philo->args, forks);
	sem_wait(philo->args.stop_program);
	exit(1);
}

int	main(int ac, char **av)
{
	t_args			args;
	t_philo			*philos;
	sem_t			*forks;

	if ((ac != 5 && ac != 6) || error_in_args(av) || fill_args(av, &args))
	{
		printf("arg error\nnumber_of_philosophers | ");
		printf("time_to_die | time_to_eat | time_to_sleep | ");
		printf("[number_of_time_each_philosopher_must_eat]\n");
		exit(EXIT_FAILURE);
	}
	forks = init_forks(&args);
	if (!forks)
		exit(EXIT_FAILURE);
	philos = init_philos(args);
	if (!philos)
	{
		destroy_forks(args, forks);
		exit(EXIT_FAILURE);
	}
	destroy_philos(args, philos);
	destroy_forks(args, forks);
}
