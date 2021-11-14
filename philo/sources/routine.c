/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivloisy <ivloisy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/14 15:14:58 by ivloisy           #+#    #+#             */
/*   Updated: 2021/11/14 20:07:17 by ivloisy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	print_status(t_ph *ph, char *s)
{
	long	time;

	time = get_time();
	if (time == -1)
		return (0);
	time = time - ph->arg->start;
	if (time >= 0 && time <= INT_MAX)
		printf("%ld %d %s\n", time, ph->id, s);
	return (1);
}

static int  check_dead(t_ph *ph)
{
	pthread_mutex_lock(&ph->arg->dead);
	if (ph->arg->d == 1)
	{
		pthread_mutex_unlock(&ph->arg->dead);
		return (1);
	}
	pthread_mutex_unlock(&ph->arg->dead);
	return (0);
}

static void	eat(t_ph *ph)
{
	if (check_dead(ph))
		return ;
	pthread_mutex_lock(&ph->r_fork);
	pthread_mutex_lock(&ph->arg->print);
	if (!check_dead(ph))
		print_status(ph, "has taken a fork");
	pthread_mutex_unlock(&ph->arg->print);
	if (check_dead(ph))
	{
		pthread_mutex_unlock(&ph->r_fork);
		return ;
	}
	pthread_mutex_lock(ph->l_fork);
	pthread_mutex_lock(&ph->arg->print);
	if (!check_dead(ph))
		print_status(ph, "has taken a fork");
	pthread_mutex_unlock(&ph->arg->print);
	if (check_dead(ph))
	{
		pthread_mutex_unlock(ph->l_fork);
		pthread_mutex_unlock(&ph->r_fork);
		return ;
	}
	pthread_mutex_lock(&ph->arg->print);
	print_status(ph, "is eating");
	pthread_mutex_unlock(&ph->arg->print);
	ph->last = get_time();
	usleep(ph->arg->time_to_eat * 1000);
/* 	if (!check_last(ph))
	{
		pthread_mutex_unlock(&ph->r_fork);
		pthread_mutex_unlock(ph->l_fork);
		return ;
	} */
	pthread_mutex_unlock(&ph->r_fork);
	pthread_mutex_unlock(ph->l_fork);
	pthread_mutex_lock(&ph->arg->print);
	print_status(ph, "is sleeping");
	pthread_mutex_unlock(&ph->arg->print);
	usleep(ph->arg->time_to_sleep * 1000);
}

void	*routine(void *philo)
{
	t_ph	*ph;

	ph = (t_ph *)philo;
	if (ph->id % 2 == 0)
		usleep(1000);
//	print_status(ph, "has taken a fork");
	while (1)
	{
		if (check_dead(ph))
			return (NULL);
		eat(ph);
/*         pthread_mutex_lock(&ph->arg->print);
        print_status(ph, "has taken a fork");
        pthread_mutex_unlock(&ph->arg->print);
		pthread_mutex_unlock(&ph->arg->dead);
        if (ph->id == 1)
        {
            pthread_mutex_lock(&ph->arg->print);
            pthread_mutex_lock(&ph->arg->dead);
            ph->arg->d = 1;
            print_status(ph, "died");
            pthread_mutex_unlock(&ph->arg->dead);
            pthread_mutex_unlock(&ph->arg->print);
        } */
	}
	return (NULL);
}
