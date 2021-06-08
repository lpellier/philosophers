/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 15:31:15 by lpellier          #+#    #+#             */
/*   Updated: 2021/06/08 15:33:22 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

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
	pthread_mutex_t	output_lock;
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
void				secure_free(void *ptr);

// init_and_destroy
t_philo				create_philo(t_info *info, \
						pthread_mutex_t *forks, int index);
t_philo				*init_philos(t_info *info, pthread_mutex_t *forks);
pthread_mutex_t		*init_forks(t_info *info);
void				join_philos(t_state *state);
void				destroy_forks(t_state *state);

// philo_does
void				philo_thinks(t_philo *philo);
void				philo_takes_forks(t_philo *philo);
void				philo_eats(t_philo *philo);
void				philo_sleeps(t_philo *philo);
void				philo_does(t_philo *philo);

// time_utils
long				time_passed(struct timeval *ref);
void				*check_time(void *arg);
void				output(t_philo *philo, char *msg);
void				better_usleep(int time);

// main
t_state				*init_state(char **av);
void				*philo_routine(void *arg);

#endif
