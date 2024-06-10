/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmart2 <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 14:42:43 by andmart2          #+#    #+#             */
/*   Updated: 2024/06/10 11:22:01 by andmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/time.h>
# include <string.h>

typedef struct s_data	t_data;
typedef struct s_philo	t_philo;

typedef struct s_philo
{
	int			id;
	int			left_eat;
	int			eating;
	int			t_die;
	t_data			*data;
	pthread_mutex_t		fork_right;
	pthread_mutex_t		*fork_left;
	pthread_mutex_t		m_t_die;
}	t_philo;

typedef struct s_data
{
	int	n_phis;
	int	t_die;
	int	t_eat;
	int	t_sleep;
	int	must_eat;
	int	t_start;
	int	end;
	int	eaten;
	t_philo	*phi;
	pthread_t	*threads;
	pthread_mutex_t	mprint;
	pthread_mutex_t	mstart;
	pthread_mutex_t	mend;
	pthread_mutex_t	meaten;
}	t_data;

/* philo.c */
int	parse_args(t_data *dara, char **argv);
int	init_data(t_data *data);
void	init_philos(t_data *data);
int	init_threads(t_data *data);

/* routine.c */
void	*routine(void *arg);
void	*alone_routine(t_philo *phi);
void	multiple_routine(t_philo *phi);
void	supervise(t_data *data);
int	supervise_eating(t_data *data);

/* time.c */
int	ft_time(void);
void	ft_sleep(int millisec);

/* utils.c */
int	ft_atoi_phi(char *str, int *arg);
int	error(char *message, t_data *data, int flag);
void	ft_clean(t_data *data, int flag);
void	ft_print(char	*message, t_philo *phi, int flag);

#endif
