/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 16:24:53 by lpellier          #+#    #+#             */
/*   Updated: 2021/06/08 15:14:59 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
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

enum		e_bool
{
	FALSE,
	TRUE
};

typedef struct s_info
{
	struct timeval	time_since_start;
	pthread_mutex_t output_lock;
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meal_goal;
	int				everyone_is_alive;
}					t_info;

typedef struct s_philo
{
	pthread_mutex_t	*adjacent_forks[2];
	pthread_t		thread;
	t_info			*info;
	struct timeval	time_since_last_meal;
	int				number_of_meals;
	int				does;
	int				philo_number;
}					t_philo;

typedef struct s_state
{
	pthread_mutex_t	*forks;
	t_philo			*philos;
	t_info			*info;
}					t_state;

// utils
void				ft_bzero(void *s, size_t n);
int					a_malloc(void **ptr, int size);
int					ft_calloc(void **ptr, size_t count, size_t size);
int					ft_atoi(const char *str);

// init_and_destroy
t_philo				create_philo(t_info *info, \
						pthread_mutex_t *forks, int index);
t_philo				*init_philos(t_info *info, pthread_mutex_t *forks);
pthread_mutex_t		*init_forks(t_info *info);
void				join_philos(t_state *state);
void				destroy_forks(t_state *state);
t_state				*init_state(char **av);

// main
void				*philo_routine(void *arg);
void				secure_free(void *ptr);

#endif
