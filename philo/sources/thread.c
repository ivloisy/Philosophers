/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivloisy <ivloisy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 16:07:24 by ivloisy           #+#    #+#             */
/*   Updated: 2021/11/13 04:04:05 by ivloisy          ###   ########.fr       */
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

	time = get_time();
	if (time == -1)
		return (0);
	time = time - ph->arg.start;
	if (time >= 0 && time <= INT_MAX)
		printf("%ld %d %s\n", time, ph->id, s);
	return (1);
}

static void	*routine(void *philo)
{
	t_ph	*ph;
//	int		i;

	ph = (t_ph *)philo;
	if (ph->id % 2 == 0)
		usleep(1000);
	print_status(ph, "has taken a fork");
/* 	if (ph->arg.number_of_times_each_philosopher_must_eat == 0)
		i = -1;
	else
		i = 0;
	while (i < ph->arg.number_of_times_each_philosopher_must_eat)
	{
		pthread_mutex_lock(&ph->r_fork);
		print_status(ph, "has taken a fork");
		pthread_mutex_lock(ph->l_fork);
		print_status(ph, "has taken a fork");
		print_status(ph, "is eating");
		usleep(ph->arg.time_to_eat * 1000);
		print_status(ph, "is sleeping");
		pthread_mutex_unlock(ph->l_fork);
		pthread_mutex_unlock(&ph->r_fork);
		usleep(ph->arg.time_to_sleep * 1000);
		print_status(ph, "is thinking");
		if (ph->arg.number_of_times_each_philosopher_must_eat != 0)
			i++;
	} */
	return (NULL);
}

int	thread(t_data *data)
{
	int		i;

	i = -1;
	data->ph = (t_ph *)malloc(sizeof(t_ph) * data->arg.number_of_philosophers);
	if (data->ph == NULL)
	{
		data->arg.error = "Memory allocation failed.";
		return (0);
	}
	data->arg.start = get_time();
	if (data->arg.start < 0)
	{
		free (data->ph);
		data->arg.error = "getimeofday failed.";
		return (0);
	}
	init_ph(data);
	while (++i < data->arg.number_of_philosophers)
	{
		data->ph[i].arg = data->arg;
		if (pthread_create(&data->ph[i].th, NULL, routine, &data->ph[i]))
		{
			data->arg.error = "pthread_create failed.";
			free (data->ph);
			return (0);
		}
	}
	finish(data);
	return (1);
}
