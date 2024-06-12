/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmart2 <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 14:42:29 by andmart2          #+#    #+#             */
/*   Updated: 2024/06/10 09:33:30 by andmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_threads(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_phis)
	{
		if (pthread_create(&data->threads[i], NULL, &routine, &data->phi[i]))
			return (i);
	}
	return (-1);
}

void	init_philos(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_phis)
	{
		data->phi[i].id = i + 1;
		data->phi[i].left_eat = data->must_eat;
		data->phi[i].eating = 0;
		data->phi[i].t_die = data->t_die;
		data->phi[i].data = data;
		pthread_mutex_init(&data->phi[i].fork_right, NULL);
		pthread_mutex_init(&data->phi[i].m_t_die, NULL);
		if (i == 0)
			data->phi[i].fork_left = &data->phi[data->n_phis - 1].fork_right;
		else
			data->phi[i].fork_left = &data->phi[i - 1].fork_right;
	}
}

int	parse_args(t_data *data, char **argv)
{
	int	i;

	i = 1;
	data->must_eat = -1;
	data->phi = NULL;
	data->threads = NULL;
	if (ft_atoi_phi(argv[i++], &data->n_phis))
		return (error("Introduce number of philosophers", data));
	if (ft_atoi_phi(argv[i++], &data->t_die))
		return (error("Introduce time to die of philosophers", data));
	if (ft_atoi_phi(argv[i++], &data->t_eat))
		return (error("Introduce time to eat of philosophers", data));
	if (ft_atoi_phi(argv[i++], &data->t_sleep))
		return (error("Introduce time to sleep", data));
	if (argv[i] && ft_atoi_phi(argv[i], &data->must_eat))
		return (error("Introduce n times each philosopher must eat", data));
	return (0);
}

int	init_data(t_data *data)
{
	int	check;

	data->end = 0;
	data->eaten = 0;
	data->phi = malloc(sizeof(t_philo) * data->n_phis);
	if (!data->phi)
		return (error("Allocation failed", data));
	pthread_mutex_init(&data->mprint, NULL);
	pthread_mutex_init(&data->mstart, NULL);
	pthread_mutex_init(&data->meaten, NULL);
	pthread_mutex_init(&data->mend, NULL);
	data->threads = malloc(sizeof(pthread_t) * (data->n_phis + 1));
	if (!data->threads)
		return (error("Allocation failed", data));
	memset(data->threads, 0, sizeof(pthread_t) * (data->n_phis + 1));
	pthread_mutex_lock(&data->mstart);
	init_philos(data);
	check = init_threads(data);
	if (check != -1)
		return (error("Error when creating threads", data));
	data->t_start = ft_time();
	pthread_mutex_unlock(&data->mstart);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	if (argc < 5 || argc > 6)
	{
		printf("Introduce 4 or 5 arguments \n");
		printf("1.Number of philosophers 2.Time to die \n");
		printf("3.Time to eat 4.Time to sleep\n");
		printf("5.Number of times each philosopher must eatoptional");
		return (1);
	}
	if (parse_args(&data, argv))
		return (1);
	if (init_data(&data))
		return (1);
	supervise(&data);
	i = -1;
	while (++i < data.n_phis)
	{
		if (pthread_join(data.threads[i], NULL))
			return (error("Error joining threads", &data));
	}
	ft_clean(&data);
	return (0);
}
