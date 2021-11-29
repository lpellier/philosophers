/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 17:13:05 by lpellier          #+#    #+#             */
/*   Updated: 2021/11/29 16:37:04 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

enum		e_actions
{
	THINK,
	FORK,
	EAT,
	SLEEP
};

typedef struct	s_args
{
    pthread_mutex_t	*output_lock;
	int				*everyone_is_alive;
	
	int				nbr_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meal_goal;
}				t_args;


typedef struct  s_philo
{
    pthread_mutex_t	*adjacent_forks[2];
	pthread_t		thread;
    t_args			args;
	struct timeval	time_since_last_meal;
	int				holding_forks;
	int				does;
	int				meals_eaten;
	int				philo_index;
}               t_philo;

// typedef struct  s_fork
// {
// 	pthread_mutex_t	*fork;
// 	int				fork_index;
// }               t_fork;

// typedef struct  s_list
// {
// 	t_philo			philo;
// 	pthread_mutex_t	*left_fork;
// 	pthread_mutex_t	*right_fork;

// 	t_list			*next;
// }               t_list;

void				better_usleep(int time);
void				philo_thinks(t_philo *philo);
void				philo_takes_forks(t_philo *philo);
void				philo_eats(t_philo *philo);
void				philo_sleeps(t_philo *philo);
long	time_passed(struct timeval *ref);
void	output(t_philo *philo, char *msg);
void	philo_does(t_philo *philo);

// main
void	*philo_routine(void *arg);

// init
pthread_mutex_t	*init_forks(t_args args);
t_philo	*init_philos(t_args args, pthread_mutex_t *forks);

// destroy
void	secure_free(void ** ptr);
void	destroy_forks(t_args args, pthread_mutex_t *forks);
void	destroy_philos(t_args args, t_philo *philos);

// utils
int	error_in_args(char **av);
int	ft_atoi(const char *str);
int	fill_args(char **av, t_args *args);

#endif
