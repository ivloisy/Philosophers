/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivloisy <ivloisy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/14 15:14:58 by ivloisy           #+#    #+#             */
/*   Updated: 2021/11/17 04:54:33 by ivloisy          ###   ########.fr       */
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
	if (time >= 0 && time <= INT_MAX && !check_stop(ph))
	{
		pthread_mutex_lock(&ph->arg->print);
		printf("%ld %d %s\n", time, ph->id, s);
		pthread_mutex_unlock(&ph->arg->print);
	}
	return (1);
}

static void	sleeping(t_ph *ph)
{
	wait_until(ph->arg->time_to_eat);
	if (ph->id % 2 == 0)
		pthread_mutex_unlock(ph->l_fork);
	else
		pthread_mutex_unlock(ph->r_fork);
	if (ph->id % 2 == 0)
		pthread_mutex_unlock(ph->r_fork);
	else
		pthread_mutex_unlock(ph->l_fork);
	print_status(ph, "is sleeping");
	wait_until(ph->arg->time_to_sleep);
	print_status(ph, "is thinking");
}

static void	eat(t_ph *ph)
{
	if (ph->id % 2 == 0)
		pthread_mutex_lock(ph->l_fork);
	else
		pthread_mutex_lock(ph->r_fork);
	print_status(ph, "has taken a fork");
	if (ph->arg->number_of_philosophers == 1)
	{
		pthread_mutex_unlock(ph->l_fork);
		wait_until(ph->arg->time_to_die);
		return ;
	}
	if (ph->id % 2 == 0)
		pthread_mutex_lock(ph->r_fork);
	else
		pthread_mutex_lock(ph->l_fork);
	print_status(ph, "has taken a fork");
	print_status(ph, "is eating");
	pthread_mutex_lock(&ph->arg->meal);
	ph->last = get_time();
	pthread_mutex_unlock(&ph->arg->meal);
	ph->nb++;
	sleeping(ph);
}

static void	say_stop(t_ph *ph)
{
	if (ph->arg->done == ph->arg->number_of_philosophers)
	{
		pthread_mutex_unlock(&ph->arg->rep);
		pthread_mutex_lock(&ph->arg->dead);
		ph->arg->stop = 1;
		pthread_mutex_unlock(&ph->arg->dead);
	}
	else
		pthread_mutex_unlock(&ph->arg->rep);
}

void	*routine(void *philo)
{
	t_ph	*ph;
//	int		i;

	ph = (t_ph *)philo;
	if (ph->id % 2 == 0)
		wait_until(ph->arg->time_to_eat / 10);
//	i = 0;
	while (!check_stop(ph))
	{
		eat(ph);
	//	i++;
		if (ph->arg->number_of_times_each_philosopher_must_eat != 0)
		{
		//	pthread_mutex_lock(&ph->n);
			if (ph->nb == ph->arg->number_of_times_each_philosopher_must_eat)
			{
			//	pthread_mutex_unlock(&ph->n);
				pthread_mutex_lock(&ph->arg->rep);
				ph->arg->done++;
				say_stop(ph);
			}
/* 			else
				pthread_mutex_lock(&ph->n); */
		}
	}
	return (NULL);
}
