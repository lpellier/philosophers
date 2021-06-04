/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 16:24:53 by lpellier          #+#    #+#             */
/*   Updated: 2021/06/04 15:51:21 by lpellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct s_state
{
	int			number_of_philosophers;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			number_of_time_each_philosopher_must_eat;
	struct timeval time_since_start;
}				t_state;

typedef struct s_philinfo
{
	pthread_mutex_t	*adjacent_forks;
	int				philo_number;
	t_state			*state;
}				t_philinfo;

// utils
void    ft_bzero(void *s, size_t n);
int     a_malloc(void **ptr, int size);
int     ft_calloc(void **ptr, size_t count, size_t size);
int     ft_atoi(const char *str);

// init_and_destroy
pthread_t	*init_threads(t_state *state, pthread_mutex_t *mutex);
pthread_mutex_t	*init_mutex(t_state *state);
void	join_threads(t_state *state, pthread_t *threads);
void	destroy_mutex(t_state *state, pthread_mutex_t *mutex);
t_state		*init_state(char **av);

// main
void	*philo_routine(void *arg);
void    secure_free(void *ptr);
