/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivloisy <ivloisy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 16:07:24 by ivloisy           #+#    #+#             */
/*   Updated: 2021/11/16 18:11:16 by ivloisy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	create_fork(t_data *data)
{
	int				i;
	pthread_mutex_t	*fork;

	i = 0;
	fork = malloc(sizeof(pthread_mutex_t) * data->arg.number_of_philosophers);
	if (fork == NULL)
	{
		data->arg.error = "Memory allocation failed.";
		return (0);
	}
	while (i < data->arg.number_of_philosophers)
	{
		if (pthread_mutex_init(&fork[i], NULL) != 0)
		{
			data->arg.error = "pthread_mutex_init failed.";
			return (0);
		}
		i++;
	}
	data->arg.fork = fork;
	return (1);
}

void	assign_fork(t_data *data, t_ph *ph)
{
	if (ph->id == data->arg.number_of_philosophers)
		ph->r_fork = &data->arg.fork[0];
	else
		ph->r_fork = &data->arg.fork[ph->id];
	ph->l_fork = &data->arg.fork[(ph->id - 1) % data->arg.number_of_philosophers];
//  ph->l_fork = &data->arg->fork[ph->id - 1];
//  printf("%d\n", ph->id);
}

static void	init_ph(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_init(&data->arg.dead, NULL);
	data->arg.stop = 0;
	pthread_mutex_init(&data->arg.meal, NULL);
	data->arg.start = get_time();
	while (i < data->arg.number_of_philosophers)
	{
		data->ph[i].id = i + 1;
		data->ph[i].last = data->arg.start;

/*		data->ph[i].l_fork = NULL;
 		pthread_mutex_init(&data->ph[i].r_fork, NULL);
		if (data->arg.number_of_philosophers == 1)
			return ;
		if (i == data->arg.number_of_philosophers - 1)
			data->ph[i].l_fork = &data->ph[0].r_fork;
		else
			data->ph[i].l_fork = &data->ph[i + 1].r_fork; */
		assign_fork(data, &data->ph[i]);
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
 */

int	thread(t_data *data)
{
	int		i;

	i = -1;
	data->ph = (t_ph *)malloc(sizeof(t_ph)
			* (data->arg.number_of_philosophers));
	if (data->ph == NULL)
	{
		data->arg.error = "Memory allocation failed.";
		return (0);
	}
/* 	data->arg.start = get_time();
	if (data->arg.start < 0)
	{
		free (data->ph);
		data->arg.error = "getimeofday failed.";
		return (0);
	} */
	if (!create_fork(data))
		return (0);
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
//	i = 0;
	finish(data/* , &i */);
	return (1);
}
