/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivloisy <ivloisy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 16:07:24 by ivloisy           #+#    #+#             */
/*   Updated: 2021/11/17 04:06:29 by ivloisy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static int	create_mutex(t_data *data)
{
	int				i;
	pthread_mutex_t	*fork;

	i = -1;
	fork = malloc(sizeof(pthread_mutex_t) * data->arg.number_of_philosophers);
	if (fork == NULL)
		return (0);
	while (++i < data->arg.number_of_philosophers)
	{
		if (pthread_mutex_init(&fork[i], NULL) != 0)
			return (0);
	}
	data->arg.fork = fork;
	if (pthread_mutex_init(&data->arg.dead, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&data->arg.meal, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&data->arg.rep, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&data->arg.print, NULL) != 0)
		return (0);
	return (1);
}

static void	assign_fork(t_data *data, t_ph *ph)
{
	if (ph->id == data->arg.number_of_philosophers)
		ph->r_fork = &data->arg.fork[0];
	else
		ph->r_fork = &data->arg.fork[ph->id];
	ph->l_fork = &data->arg.fork[(ph->id - 1)
		/* % data->arg.number_of_philosophers */];
}

static int	init_ph(t_data *data)
{
	int	i;

	i = 0;
	data->arg.stop = 0;
	data->arg.done = 0;
	data->arg.start = get_time();
	while (i < data->arg.number_of_philosophers)
	{
		data->ph[i].id = i + 1;
		data->ph[i].last = data->arg.start;
		data->ph[i].nb = 0;
		assign_fork(data, &data->ph[i]);
		if (pthread_mutex_init(&data->ph[i].n, NULL) != 0)
			return (0);
		i++;
	}
	return (1);
}

int	thread(t_data *data)
{
	int		i;

	i = -1;
	data->ph = (t_ph *)malloc(sizeof(t_ph)
			* (data->arg.number_of_philosophers));
	if (data->ph == NULL)
		return (0);
	if (!create_mutex(data))
		return (0);
	if (!init_ph(data))
		return (0);
	while (++i < data->arg.number_of_philosophers)
	{
		data->ph[i].arg = &data->arg;
		if (pthread_create(&data->ph[i].th, NULL, routine, &data->ph[i]))
		{
			free (data->ph);
			data->ph = NULL;
			free (data->arg.fork);
			data->arg.fork = NULL;
			return (0);
		}
	}
	finish(data);
	return (1);
}
