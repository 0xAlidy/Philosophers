/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alidy <alidy@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 08:35:30 by alidy             #+#    #+#             */
/*   Updated: 2021/05/24 11:05:27 by alidy            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	print_state(t_ph *ph, int id, int state)
{
	pthread_mutex_lock(&(ph->speak));
	if (phi_is_dead(ph) == 1)
	{
		pthread_mutex_unlock(&(ph->speak));
		return ;
	}
	if (state == EATING)
		printf("%-5ld %d is eating\n", ft_timer(ph->time), id);
	else if (state == SLEEPING)
		printf("%-5ld %d is sleeping\n", ft_timer(ph->time), id);
	else if (state == THINKING)
		printf("%-5ld %d is thinking\n", ft_timer(ph->time), id);
	else if (state == FORK)
		printf("%-5ld %d has taken a fork\n", ft_timer(ph->time), id);
	else if (state == DIED)
	{
		ph->is_dead = 1;
		printf("%-5ld %d died\n", ft_timer(ph->time), id);
	}
	pthread_mutex_unlock(&(ph->speak));
}

t_philo	init_philo(t_ph *ph)
{
	t_philo	philo;

	philo.id = ph->current + 1;
	pthread_mutex_unlock(&(ph->id));
	philo.nb_eat = 0;
	philo.state = EATING;
	gettimeofday(&(philo.last_eat), NULL);
	return (philo);
}

int	fork_id(int nb, int id)
{
	int	res;

	if (id == 1)
		res = nb - 1;
	else
		res = id - 2;
	return (res);
}

int	phi_is_dead(t_ph *ph)
{
	int	res;

	res = 0;
	pthread_mutex_lock(&(ph->dead));
	if (ph->is_dead == 1)
		res = 1;
	pthread_mutex_unlock(&(ph->dead));
	return (res);
}

void	free_ph(t_ph *ph)
{
	pthread_mutex_destroy(&(ph->id));
	pthread_mutex_destroy(&(ph->dead));
	pthread_mutex_destroy(&(ph->speak));
	pthread_mutex_destroy(&(ph->m_fork));
	free(ph->forks);
}
