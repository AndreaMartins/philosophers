#include "philo.h"

int	ft_time(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return((tv.tv_sec * 1000) + (tv.tv_usec /1000));
}

void	ft_sleep(int millisec)
{
	int end;

	end = ft_time() + millisec;
	while (ft_time() < end)
		usleep(200);
}
