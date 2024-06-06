#include "philo.h"

void	*routine(void *arg)
{
	t_philo *phi;

	int	end;

	phi = (t_philo *)arg;
	pthread_mutex_lock(&phi->data->mstart);
	pthread_mutex_unlock(&phi->data->mstart);

	if (phi->id % 2 == 0)
		ft_sleep(1);
	if (phi->data->must_eat == 0)
		return((void *)0);
	if (phi->data->n_phis == 1)
		return(alone_routine(phi));
	pthread_mutex_lock(&phi->data->mend);
	end = phi->data->end;
	pthread_mutex_unlock(&phi->data->mend);
	while(!end)
	{
		multiple_routine(phi);
		pthread_mutex_lock(&phi->data->mend);
		end = phi->data->end;
		pthread_mutex_unlock(&phi->data->mend);
	}
	return NULL;
}

void	*alone_routine(t_philo *phi)
{
	pthread_mutex_lock(&phi->fork_right);
	ft_print("has taken a right fork", phi, 0);
	ft_sleep(phi->t_die);
	pthread_mutex_unlock(&phi->fork_right);
	return((void *)0);
}

void	multiple_routine(t_philo *phi)
{
	pthread_mutex_lock(&phi->fork_right);
	pthread_mutex_lock(phi->fork_left);
	ft_print("has taken a right fork", phi, 0);
	ft_print("has taken a left fortk", phi, 0);
	pthread_mutex_lock(&phi->m_t_die);
	
	phi->t_die = ft_time() - phi->data->t_start + phi->data->t_die;
	phi->eating = 1;

	pthread_mutex_unlock(&phi->m_t_die);
	ft_print("is eating", phi, 0);
	ft_sleep(phi->data->t_eat);
	phi->left_eat--;
	if(phi->left_eat == 0)
	{
		pthread_mutex_lock(&phi->data->meaten);
		phi->data->eaten++;
		pthread_mutex_unlock(&phi->data->meaten);
	}
	phi->eating = 0;
	pthread_mutex_unlock(&phi->fork_right);
	pthread_mutex_unlock(phi->fork_left);
	ft_print("is sleeping", phi, 0);
	ft_sleep(phi->data->t_sleep);
	ft_print("is thinking", phi, 0);
}

void	supervise(t_data *data)
{
	int	i;
	int	time;

	i = -1;
	while(++i < data->n_phis)
	{
		pthread_mutex_lock(&data->phi[i].m_t_die);
		time = data->phi[i].t_die;
		pthread_mutex_unlock(&data->phi[i].m_t_die);
		if(time <= (ft_time() - data-> t_start) && !data->phi[i].eating)
		{
			pthread_mutex_lock(&data->mend);
			data->end = 1;
			pthread_mutex_unlock(&data->mend);
			ft_print("died", &data->phi[i],1);
			break;
		}
		if(data->must_eat > 0 && supervise_eating(data))
			break;
		if(i == data->n_phis - 1)
			i = -1;
	}
}

int	supervise_eating(t_data *data)
{
	int	eating_count;

	pthread_mutex_lock(&data->meaten);
	eating_count = data->eaten;
	pthread_mutex_unlock(&data->meaten);
	if(eating_count == data->n_phis && data->must_eat >= 0)
	{
		pthread_mutex_lock(&data->mend);
		data->end = 1;
		pthread_mutex_unlock(&data->mend);
		return(1);
	}
	return(0);
}
