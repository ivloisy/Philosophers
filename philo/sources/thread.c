/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivloisy <ivloisy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 16:07:24 by ivloisy           #+#    #+#             */
/*   Updated: 2021/11/12 23:07:17 by ivloisy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static void	init_ph(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->arg.number_of_philosophers)
	{
		data->ph[i].id = i + 1;
		data->ph[i].l_fork = NULL;
		pthread_mutex_init(&data->ph[i].r_fork, NULL);
		if (data->arg.number_of_philosophers == 1)
			return ;
		if (i == data->arg.number_of_philosophers - 1)
			data->ph[i].l_fork = &data->ph[0].r_fork;
		else
			data->ph[i].l_fork = &data->ph[i + 1].r_fork;
		i++;
	}
}

static int	print_status(t_ph *ph, char *s)
{
	long	time;

	time = get_time(ph);
	if (time == -1)
		return (0);
	time = time - ph->arg.start;
	if (time >= 0 && time <= INT_MAX)
		printf("%ld %d %s\n", time, ph->id, s);
	return (1);
}

static void	*routine(void *philo)
{
//	(void)ph;
	t_ph	*ph;

	ph = (t_ph *)philo;
	if (ph->id % 2 == 0)
		usleep(100);
/* 	while (1)
	{ */
		pthread_mutex_lock(&ph->r_fork);
		print_status(ph, "has taken a fork");
		pthread_mutex_lock(ph->l_fork);
		print_status(ph, "has taken a fork");
		print_status(ph, "is eating");
		usleep(ph->arg.time_to_eat * 1000);
		pthread_mutex_unlock(ph->l_fork);
		print_status(ph, "is sleeping");
		pthread_mutex_unlock(&ph->r_fork);
//	}
	
	return (NULL);
}

int thread(t_data *data)
{
//	t_ph	*ph;
	int		i;

	data->ph = (t_ph *)malloc(sizeof(t_ph) * data->arg.number_of_philosophers);
	if (data->ph == NULL)
	{
		data->arg.error = "Memory allocation failed.";
		return (0);
	}
	data->arg.start = get_time();
	if (data->arg.start < 0)
	{
		data->arg.error = "getimeofday failed.";
		return (0);
	}
//	printf ("%ld\n", data->arg.start);///
 	init_ph(data);
	i = 0;
	while (i < data->arg.number_of_philosophers)
	{
		data->ph[i].arg = data->arg;
		if (pthread_create(&data->ph[i].th, NULL, routine, &data->ph[i]))
		{
			print_error("pthread_create failed.");
			return (0);
		}
		i++;
	}
	i = 0;
	while (i < data->arg.number_of_philosophers)
	{
		pthread_join(data->ph[i].th, NULL);
		i++;
	}
	return (1);
}