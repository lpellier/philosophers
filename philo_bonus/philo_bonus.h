/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 15:31:15 by lpellier          #+#    #+#             */
/*   Updated: 2021/11/12 17:06:48 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <sys/types.h>
# include <semaphore.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <signal.h>

enum		e_actions
{
	THINK,
	FORK,
	EAT,
	SLEEP
};

typedef struct s_info
{
	struct timeval	time_since_start;
	int				nbr_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meal_goal;
}					t_info;

typedef struct s_philo
{
	pid_t			cpid;
	pthread_t		thread;
	t_info			*info;
	struct timeval	time_since_last_meal;
	int				is_alive;
	int				number_of_meals;
	int				does;
	int				philo_number;
}					t_philo;

typedef struct s_process
{
	t_philo			*philos;
	int				philo_index;
	int				nbr_of_philos;
}					t_process;

typedef struct s_state
{
	sem_t			*lock;
	sem_t			*forks;
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
t_philo				create_philo(t_info *info, int index);
t_philo				*init_philos(t_info *info);
void				init_forks(t_state *state);
void				join_philos(t_state *state);
void				destroy_forks(t_state *state);

// process_init
void				kill_all_processes(t_process *process);
void				*philo_process(void *arg);

// philo_does
void				philo_thinks(t_philo *philo);
void				philo_takes_forks(t_philo *philo, sem_t *forks);
void				philo_eats(t_philo *philo, sem_t *forks);
void				philo_sleeps(t_philo *philo);
void				philo_does(t_philo *philo, sem_t *forks);

// time_utils
long				time_passed(struct timeval *ref);
void				*check_time(void *arg);
void				output(t_philo *philo, char *msg);
void				better_usleep(int time);

// main
t_state				*init_state(char **av);
int					philo_routine(t_philo *philo);
int					error_in_args(char **av);

#endif
