/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alidy <alidy@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 13:44:52 by alidy             #+#    #+#             */
/*   Updated: 2021/02/04 10:51:17 by alidy            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	init_philo(t_prm *phi, int argc, char **argv)
{
	if (argc != 5 && argc != 6)
	{
		dprintf(1, "test\n");
		write(1, "Bad args\n", 9);
		return (0);
	}
	phi->nb = ft_atoi(argv[1]);
	phi->die = ft_atoi(argv[2]);
	phi->eat = ft_atoi(argv[3]);
	phi->sleep = ft_atoi(argv[4]);
	if (argc == 6)
		phi->must_eat = ft_atoi(argv[5]);
	else
		phi->must_eat = -1;
	dprintf(1,"nb: %d die: %d eat: %d sleep: %d must eat: %d\n"
	, phi->nb, phi->die, phi->eat, phi->sleep, phi->must_eat);
	return (1);
}

void	*start_routine(void *nb)
{
	int nu;

	nu = *(int *)nb;
	dprintf(1,"Je suis le thread n°%d\n", nu);
	sleep(1 + nu);
	dprintf(1,"ok\n");
	return NULL;
}

void	philosophers(t_prm *phi)
{
	int 		i;
	pthread_t 	thread_id[phi->nb];

	i = 0;
	while (i < phi->nb)
	{
		if (pthread_create(&thread_id[i], NULL, start_routine, &i))
			ft_write("Erreur à la création d'un thread\n");
		sleep(1);
		++i;
	}
	/*i = 0;
	while (i < phi->nb)
	{
		pthread_join(thread_id[i], NULL);
		ft_write("wait\n");
		++i;
	}*/
}

int		main(int argc, char **argv)
{
	t_prm phi;

	if (init_philo(&phi, argc, argv))
		philosophers(&phi);
	return (0);
}
