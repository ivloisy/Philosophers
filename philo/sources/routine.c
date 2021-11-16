/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivloisy <ivloisy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/14 15:14:58 by ivloisy           #+#    #+#             */
/*   Updated: 2021/11/16 19:14:04 by ivloisy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	print_status(t_ph *ph, char *s)
{
	long	time;

	time = get_time();
	if (time == -1)
		return (0);
	pthread_mutex_lock(&ph->arg->dead);
	time = time - ph->arg->start;
	if (time >= 0 && time <= INT_MAX && !ph->arg->stop)
		printf("%ld %d %s\n", time, ph->id, s);
	pthread_mutex_unlock(&ph->arg->dead);
	return (1);
}

int	check_stop(t_ph *ph)
{
	pthread_mutex_lock(&ph->arg->dead);
	if (ph->arg->stop)
	{
		pthread_mutex_unlock(&ph->arg->dead);
		return (1);
	}
	pthread_mutex_unlock(&ph->arg->dead);
	return (0);
}

static void	eat(t_ph *ph)
{
	if (ph->id % 2 == 0)
		pthread_mutex_lock(ph->l_fork);
	else
		pthread_mutex_lock(ph->r_fork);
	print_status(ph, "has taken a fork left");
	if (ph->arg->number_of_philosophers == 1)
	{
		pthread_mutex_unlock(ph->l_fork);
		usleep(ph->arg->time_to_die * 1000);
		return ;
	}
	if (ph->id % 2 == 0)
		pthread_mutex_lock(ph->r_fork);
	else
		pthread_mutex_lock(ph->l_fork);
	print_status(ph, "has taken a fork right");
	print_status(ph, "is eating");
	pthread_mutex_lock(&ph->arg->meal);
	ph->last = get_time();
	pthread_mutex_unlock(&ph->arg->meal);
	usleep(ph->arg->time_to_eat * 1000);
	if (ph->id % 2 == 0)
		pthread_mutex_unlock(ph->l_fork);
	else
		pthread_mutex_unlock(ph->r_fork);
	if (ph->id % 2 == 0)
		pthread_mutex_unlock(ph->r_fork);
	else
		pthread_mutex_unlock(ph->l_fork);
	print_status(ph, "is sleeping");
	usleep(ph->arg->time_to_sleep * 1000);
	print_status(ph, "is thinking");
}


void	*routine(void *philo)
{
	t_ph	*ph;

	ph = (t_ph *)philo;
	if (ph->id % 2 == 0)
		usleep(1000);
	while (!check_stop(ph))
	{
		eat(ph);
	}
	return (NULL);
}
