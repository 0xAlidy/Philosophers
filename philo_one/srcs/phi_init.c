/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alidy <alidy@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 08:32:42 by alidy             #+#    #+#             */
/*   Updated: 2021/02/24 08:58:27 by alidy            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	test_args(int argc, char **argv)
{
	int i;
	int y;

	i = 1;
	y = 0;
	if (argc != 5 && argc != 6)
	{
		printf("Bad args\n");
		return (0);
	}
	while (i < argc)
	{
		while (argv[i][y])
		{
			if (!(argv[i][y] >= '0' && argv[i][y] <= '9'))
			{
				printf("Bad args\n");
				return (0);
			}
			++y;
		}
		y = 0;
		++i;
	}
	return (1);
}

int	init_mutex(t_ph *ph)
{
	int i;

	i = 0;
	if (!(ph->forks = malloc(sizeof(int) * ph->nb)))
		return (0);
	while (i < ph->nb)
	{
		ph->forks[i] = 0;
		++i;
	}
	pthread_mutex_init(&(ph->dead), NULL);
	pthread_mutex_init(&(ph->speak), NULL);
	pthread_mutex_init(&(ph->id), NULL);
	pthread_mutex_init(&(ph->m_fork), NULL);
	return (1);
}

int	init_ph(t_ph *ph, int argc, char **argv)
{
	if (!test_args(argc, argv))
		return (0);
	if (argc == 6)
	{
		ph->must_eat = ft_atoi(argv[5]);
		if (ph->must_eat < 1)
			return (0);
	}
	else
		ph->must_eat = -1;
	ph->nb = ft_atoi(argv[1]);
	ph->t_die = ft_atoi(argv[2]);
	ph->t_eat = ft_atoi(argv[3]);
	ph->t_sleep = ft_atoi(argv[4]);
	ph->is_dead = 0;
	if (!init_mutex(ph))
		return (0);
	return (1);
}
