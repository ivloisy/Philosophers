/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivloisy <ivloisy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 13:36:02 by ivloisy           #+#    #+#             */
/*   Updated: 2021/11/17 01:19:49 by ivloisy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static int	print_error(char *s)
{
	if (!s)
		printf("Error.\n");
	else
		printf("Error : %s\n", s);
	return (0);
}

static void	init(char **av, t_data *data, int i)
{
	data->arg.number_of_times_each_philosopher_must_eat = 0;
	if (i == 1)
		data->arg.number_of_philosophers = ft_atoi(av[i]);
	else if (i == 2)
		data->arg.time_to_die = ft_atoi(av[i]);
	else if (i == 3)
		data->arg.time_to_eat = ft_atoi(av[i]);
	else if (i == 4)
		data->arg.time_to_sleep = ft_atoi(av[i]);
	else if (i == 5)
		data->arg.number_of_times_each_philosopher_must_eat = ft_atoi(av[i]);
}

static int	check_arg(int ac, char **av, t_data *data)
{
	int		i;
	int		j;

	i = 0;
	while (++i < ac)
	{
		j = 0;
		while (av[i][j])
		{
			while (av[i][j] > 0 && av[i][j] < 33)
				j++;
			if ((av[i][j] == '-' && ft_isdigit(av[i][j + 1]))
					|| (av[i][j] == '+' && ft_isdigit(av[i][j + 1])))
				j++;
			while (ft_isdigit(av[i][j]))
				j++;
			while (av[i][j] > 0 && av[i][j] < 33)
				j++;
			if (av[i][j] != '\0' || ft_atol(av[i]) > INT_MAX
					|| ft_atol(av[i]) < INT_MIN)
				return (0);
		}
		init(av, data, i);
	}
	return (1);
}

static int	parsing(int ac, char **av, t_data *data)
{
	int	i;
	int	j;
	int	k;

	if (ac < 5 || ac > 6)
		return (0);
	i = 0;
	while (++i < ac)
	{
		if (av[i][0] == '\0')
			return (0);
		j = -1;
		k = 0;
		while (av[i][++j])
		{
			if (ft_isdigit(av[i][j]) == 1)
				k = 1;
		}
		if (k == 0)
			return (0);
	}
	if (!check_arg(ac, av, data))
		return (0);
	return (1);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (!parsing(ac, av, &data))
		return (print_error("Invalid argument."));
	if (data.arg.number_of_philosophers < 1 || data.arg.time_to_die < 1
		|| data.arg.time_to_eat < 1 || data.arg.time_to_sleep < 1
		|| (data.arg.number_of_times_each_philosopher_must_eat < 1 && ac == 6))
		return (print_error("Invalid argument."));
	if (!thread(&data))
		return (print_error("Memory allocation or a pthread function failed."));
	return (1);
}
