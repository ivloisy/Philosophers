/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivloisy <ivloisy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/13 03:47:36 by ivloisy           #+#    #+#             */
/*   Updated: 2021/11/14 20:04:15 by ivloisy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	finish(t_data *data)
{
	int	i;

	i = 0;
	
	while (i < data->arg.number_of_philosophers)
	{
		pthread_join(data->ph[i].th, NULL);
		i++;
	}
	free (data->ph);
}
