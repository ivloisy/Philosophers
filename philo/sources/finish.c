/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivloisy <ivloisy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/13 03:47:36 by ivloisy           #+#    #+#             */
/*   Updated: 2021/11/16 16:42:36 by ivloisy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	check_dead(t_data *data, int *x)
{
	int	i;

	i = -1;
	while (++i < data->arg.number_of_philosophers)
	{
		pthread_mutex_lock(&data->arg.meal);
		if ((get_time() - data->ph[i].last) >= (long)(data->arg.time_to_die)
			/* && !data->philo[i].finish */)
		{
//			pthread_mutex_lock(&data->arg.print);
			pthread_mutex_unlock(&data->arg.meal);
			print_status(&data->ph[i], "died");
//			pthread_mutex_unlock(&data->arg.print);
//			check_dead(&data->ph[i], 1);
			pthread_mutex_lock(&data->arg.dead);
			data->arg.stop = 1;
			pthread_mutex_unlock(&data->arg.dead);
			break ;
		}
		pthread_mutex_unlock(&data->arg.meal);
	}
	pthread_mutex_lock(&data->arg.dead);
	if (data->arg.stop)
	{
		pthread_mutex_unlock(&data->arg.dead);
		*x = 1;
	}
}

void	finish(t_data *data)
{
	int	i;
	int	x;

	x = 0;
	while (!x)
	{
		check_dead(data, &x);
		if (!x)
			pthread_mutex_unlock(&data->arg.dead);
		usleep(1);
	}
	i = -1;
	while (++i < data->arg.number_of_philosophers)
		pthread_join(data->ph[i].th, NULL);
	pthread_mutex_destroy(&data->arg.dead);
	pthread_mutex_destroy(&data->arg.meal);
	i = -1;
	while (++i < data->arg.number_of_philosophers)
		pthread_mutex_destroy(&data->arg.fork[i]);
	free (data->ph);
	data->ph = NULL;
	free (data->arg.fork);
	data->arg.fork = NULL;
}
