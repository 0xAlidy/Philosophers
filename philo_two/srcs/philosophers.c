/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alidy <alidy@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 13:44:52 by alidy             #+#    #+#             */
/*   Updated: 2021/05/13 09:48:16 by alidy            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	phi_sleep(t_ph *ph, t_philo *philo)
{
	print_state(ph, philo->id, SLEEPING);
	if (ph->t_die < ft_have_time(&(philo->last_eat), ph->t_eat))
	{
		phi_my_sleep(ph->t_die - ft_timersub(&(philo->last_eat)));
		print_state(ph, philo->id, DIED);
	}
	else
	{
		phi_my_sleep(ph->t_sleep);
		print_state(ph, philo->id, THINKING);
		philo->state = EATING;
	}
}

void	phi_eat(t_ph *ph, t_philo *philo)
{
	sem_wait(ph->fork);
	sem_wait(ph->fork);
	print_state(ph, philo->id, FORK);
	print_state(ph, philo->id, FORK);
	print_state(ph, philo->id, EATING);
	gettimeofday(&(philo->last_eat), NULL);
	phi_my_sleep(ph->t_eat);
	sem_post(ph->fork);
	sem_post(ph->fork);
	philo->nb_eat += 1;
	philo->state = SLEEPING;
}

void	*start_routine(void *p)
{
	t_ph	*ph;
	t_philo	philo;

	ph = p;
	philo = init_philo(ph);
	while (!phi_is_dead(ph) && (ph->must_eat == -1
			|| philo.nb_eat < ph->must_eat))
	{
		usleep(10);
		if (ph->t_die < ft_timersub(&(philo.last_eat)))
		{
			phi_my_sleep(ph->t_die - ft_timersub(&(philo.last_eat)));
			print_state(ph, philo.id, DIED);
		}
		else if (philo.state == EATING)
		{
			phi_eat(ph, &philo);
		}
		else
		{
			phi_sleep(ph, &philo);
		}
	}
	return (NULL);
}

void	philosophers(t_ph *ph)
{
	int				i;
	pthread_t		*thread_id;
	struct timeval	temp;

	i = 0;
	gettimeofday(&temp, NULL);
	ph->time = ft_time(&temp);
	thread_id = malloc(sizeof(pthread_t) * ph->nb);
	if (!thread_id)
		return ;
	while (i < ph->nb)
	{
		sem_wait(ph->id);
		ph->current = i;
		if (pthread_create(&thread_id[i], NULL, start_routine, ph))
			return ;
		++i;
	}
	i = 0;
	while (i < ph->nb)
	{
		pthread_join(thread_id[i], NULL);
		++i;
	}
	free(thread_id);
}

int	main(int argc, char **argv)
{
	t_ph	ph;

	if (init_ph(&ph, argc, argv))
	{
		philosophers(&ph);
		free_ph(&ph);
	}
	return (0);
}
