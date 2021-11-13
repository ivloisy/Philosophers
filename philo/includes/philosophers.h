/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivloisy <ivloisy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 13:48:58 by ivloisy           #+#    #+#             */
/*   Updated: 2021/11/13 03:53:05 by ivloisy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <limits.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_arg
{
	int		number_of_philosophers;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		number_of_times_each_philosopher_must_eat;
	char	*error;
	long	start;
}t_arg;

typedef struct s_ph
{
	int				id;
	t_arg			arg;
	pthread_t		th;
	pthread_mutex_t	r_fork;
	pthread_mutex_t	*l_fork;
}t_ph;

typedef struct s_data
{
	t_arg	arg;
	t_ph	*ph;
}t_data;

int		thread(t_data *data);
int		print_error(char *s);
void	finish(t_data *t_data);
int		ft_isdigit(int c);
int		ft_atoi(const char *str);
long	ft_atol(const char *str);
char	*ft_strdup(const char *s1);
long	get_time(void);

#endif