/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivloisy <ivloisy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 16:07:24 by ivloisy           #+#    #+#             */
/*   Updated: 2021/11/14 20:11:08 by ivloisy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static void	init_ph(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_init(&data->arg.dead, NULL);
	data->arg.d = 0;
	pthread_mutex_init(&data->arg.print, NULL);
	while (i < data->arg.number_of_philosophers)
	{
		data->ph[i].id = i + 1;
		data->ph[i].l_fork = NULL;
		data->ph[i].last = data->arg.start;
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
/*
static int	check_last(t_ph *ph)
{
	pthread_mutex_lock(&ph->arg->dead);
	if ((get_time() - ph->last) > ph->arg->time_to_die)
	{
		ph->arg->d = 1;
		pthread_mutex_lock(&ph->arg->print);
		print_status(ph, "died");
		pthread_mutex_unlock(&ph->arg->print);
		pthread_mutex_unlock(&ph->arg->dead);
		return (0);
	}
	pthread_mutex_unlock(&ph->arg->dead);
	return (1);
}

 static void	*supervisor(void *d)
{
	t_data	*data;
	int		i;

	data = (t_data *)d;
	while (1)
	{
		i = 0;
		while (i < data->arg.number_of_philosophers)
		{
			if (!check_last(&data->ph[i]))
				return (NULL);
			i++;
		}
	}
	return (NULL);
} */

int	thread(t_data *data)
{
	int		i;

	i = -1;
	data->ph = (t_ph *)malloc(sizeof(t_ph) * (data->arg.number_of_philosophers));
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
		data->ph[i].arg = &data->arg;
		if (pthread_create(&data->ph[i].th, NULL, routine, &data->ph[i]))
		{
			data->arg.error = "pthread_create failed.";
			free (data->ph);
			return (0);
		}
	}
/* 	if (pthread_create(&data->supervisor, NULL, supervisor, &data))
	{
		data->arg.error = "pthread_create failed.";
		free (data->ph);
		return (0);
	}
	pthread_join(data->supervisor, NULL); */
	finish(data);
	return (1);
}
