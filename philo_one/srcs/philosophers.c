/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alidy <alidy@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 13:44:52 by alidy             #+#    #+#             */
/*   Updated: 2021/02/24 09:15:16 by alidy            ###   ########lyon.fr   */
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
	pthread_mutex_lock(&(ph->m_fork));
	if (!ph->forks[philo->id - 1] && !ph->forks[fork_id(ph->nb, philo->id)]
	&& ph->nb != 1)
	{
		ph->forks[philo->id - 1] = 1;
		ph->forks[fork_id(ph->nb, philo->id)] = 1;
		pthread_mutex_unlock(&(ph->m_fork));
		print_state(ph, philo->id, FORK);
		print_state(ph, philo->id, FORK);
		philo->nb_eat += 1;
		print_state(ph, philo->id, EATING);
		gettimeofday(&(philo->last_eat), NULL);
		phi_my_sleep(ph->t_eat);
		philo->state = SLEEPING;
		pthread_mutex_lock(&(ph->m_fork));
		ph->forks[philo->id - 1] = 0;
		ph->forks[fork_id(ph->nb, philo->id)] = 0;
	}
	pthread_mutex_unlock(&(ph->m_fork));
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
	pthread_t		thread_id[ph->nb];
	struct timeval	temp;

	i = 0;
	gettimeofday(&temp, NULL);
	ph->time = ft_time(&temp);
	while (i < ph->nb)
	{
		pthread_mutex_lock(&(ph->id));
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
}

int		main(int argc, char **argv)
{
	t_ph ph;

	if (init_ph(&ph, argc, argv))
	{
		philosophers(&ph);
		free_ph(&ph);
	}
	return (0);
}
