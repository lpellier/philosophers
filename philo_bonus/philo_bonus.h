#ifndef PHILO_H
# define PHILO_H

# define INTMAX_LEN 10

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
# include <limits.h>

enum		e_actions
{
	THINK,
	FORK,
	EAT,
	SLEEP
};

typedef struct s_args
{
	sem_t		*output_lock;
	sem_t		*lock;
	sem_t		*stop_program;
	int			*everyone_is_alive;

	int			nbr_of_philos;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			meal_goal;
}				t_args;

typedef struct s_philo
{
	pid_t			cpid;
	t_args			args;
	struct timeval	time_since_last_meal;
	int				holding_forks;
	int				does;
	int				meals_eaten;
	int				philo_index;
}				t_philo;

// main
int					philo_routine(void *arg);
void				loop_routine(int *everyone_alive, t_philo *philo, sem_t *forks);
void				*check_time(void *arg);
void				check_philo_death(t_philo *philo, int everyone_alive);

// philo utils
void				output(t_philo *philo, char *msg);
long				time_passed(struct timeval *ref);
int					fill_args(char **av, t_args *args);
void				better_usleep(int time);
int					error_in_args(char **av);

// philo does
void				philo_does(t_philo *philo, sem_t *forks);
void				philo_thinks(t_philo *philo);
void				philo_takes_forks(t_philo *philo, sem_t *forks);
void				philo_eats(t_philo *philo, sem_t *forks);
void				philo_sleeps(t_philo *philo);

// init
sem_t				*init_forks(t_args *args);
t_philo				create_philo(t_args args, int index);
t_philo				*init_philos(t_args args);

// destroy
void				secure_free(void **ptr);
void				destroy_forks(t_args args, sem_t *forks);
void				destroy_philos(t_args args, t_philo *philos);

// utils
int					ft_strlen(char *str);
long				ft_atoi(const char *str);

#endif
