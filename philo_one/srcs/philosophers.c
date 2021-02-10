/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alidy <alidy@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 13:44:52 by alidy             #+#    #+#             */
/*   Updated: 2021/02/10 12:56:15 by alidy            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int		init_ph(t_ph *ph, int argc, char **argv)
{
	if (argc != 5 && argc != 6)
	{
		printf("Bad args\n");
		return (0);
	}
	ph->nb = ft_atoi(argv[1]);
	ph->t_die = ft_atoi(argv[2]);
	ph->t_eat = ft_atoi(argv[3]);
	ph->t_sleep = ft_atoi(argv[4]);
	ph->is_dead = 0;
	ph->time = 0;
	if (!(ph->forks = malloc(sizeof(pthread_mutex_t) * ph->nb)))
		return (0);
	pthread_mutex_init(&(ph->dead), NULL);
	pthread_mutex_init(&(ph->speak), NULL);
	if (argc == 6)
	{
		ph->must_eat = ft_atoi(argv[5]);
		if (ph->must_eat < 1)
			return (0);
	}
	else
		ph->must_eat = -1;
	printf("nb: %d die: %d eat: %d sleep: %d must eat: %d\n", ph->nb, ph->t_die, ph->t_eat, ph->t_sleep, ph->must_eat);
	return (1);
}

void		print_state(t_ph *ph, t_philo *philo, int state)
{
	pthread_mutex_lock(&(ph->speak));
	ft_timersub(&(philo->last_eat), ph);
	if (state == EATING)
		printf("%-5lld %d is eating\n", ph->time, philo->id);
	else if (state == SLEEPING)
		printf("%-5lld %d is sleeping\n", ph->time, philo->id);
	else if (state == THINKING)
		printf("%-5lld %d is thinking\n", ph->time, philo->id);
	else if (state == FORK)
		printf("%-5lld %d has taken a fork\n", ph->time, philo->id);
	else if (state == DIED)
		printf("%-5lld %d died\n", ph->time, philo->id);
	pthread_mutex_unlock(&(ph->speak));
}

t_philo		init_philo(t_ph *ph)
{
	t_philo 		philo;

	philo.id = ph->current + 1;
	philo.nb_eat = 0;
	philo.state = EATING;
	gettimeofday(&(philo.last_eat), NULL);
	return (philo);
}

int		fork_id(t_ph *ph, int id)
{
	if (id == 1)
		return (ph->nb - 1);
	else
		return (id - 2);
}

void	free_ph(t_ph *ph)
{
	int i;

	i = 0;
	while (i < ph->nb)
	{
		pthread_mutex_destroy(&(ph->forks[i]));
		++i;
	}
	pthread_mutex_destroy(&(ph->dead));
	pthread_mutex_destroy(&(ph->speak));
}

void	*start_routine(void *p)
{	
	t_ph	*ph;
	t_philo	philo;

	ph = p;
	philo = init_philo(ph);
	pthread_mutex_lock(&(ph->dead));
	while (ph->is_dead == 0 || (ph->must_eat > 0 &&  philo.nb_eat < ph->must_eat))
	{
		pthread_mutex_unlock(&(ph->dead));
		if (philo.state == EATING)
		{
			print_state(ph, &philo, THINKING);
			pthread_mutex_lock(&(ph->forks[fork_id(ph, philo.id)]));
			print_state(ph, &philo, FORK);
			pthread_mutex_lock(&(ph->forks[philo.id - 1]));
			print_state(ph, &philo, FORK);
			if (ph->t_die <= ft_timersub(&(philo.last_eat), ph))
			{
				ph->is_dead = 1;
				print_state(ph, &philo, DIED);
			}
			else
			{
				print_state(ph, &philo, EATING);
				usleep(ph->t_eat);
				gettimeofday(&(philo.last_eat), NULL);
			}
			pthread_mutex_unlock(&(ph->forks[philo.id - 1]));
			pthread_mutex_unlock(&(ph->forks[fork_id(ph, philo.id)]));
			philo.state = SLEEPING;
		}
		else
		{
			print_state(ph, &philo, SLEEPING);
			usleep(ph->t_sleep);
			philo.state = EATING;
		}
		pthread_mutex_lock(&(ph->dead));
	}
	pthread_mutex_unlock(&(ph->dead));
	return (NULL);
}

void	philosophers(t_ph *ph)
{
	int 		i;
	pthread_t 	thread_id[ph->nb];

	i = 0;
	while (i < ph->nb)
	{
		ph->current = i; 
		pthread_mutex_init(&(ph->forks[i]), NULL);
		if (pthread_create(&thread_id[i], NULL, start_routine, ph))
		{
			printf("Erreur à la création d'un thread\n");
			return ;
		}
		++i;
		usleep(10); // creer un mutex pour proteger la creation du i ? ou sleep
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
