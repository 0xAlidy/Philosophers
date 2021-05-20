/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alidy <alidy@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 08:35:30 by alidy             #+#    #+#             */
/*   Updated: 2021/05/20 11:07:44 by alidy            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	print_state(t_ph *ph, int id, int state)
{
	sem_wait(ph->speak);
	if (phi_is_dead(ph) == 1)
	{
		sem_post(ph->speak);
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
		printf("%-5ld %d died\n", ft_timer(ph->time), id);
		sem_wait(ph->is_dead);
		*(ph->dead) = -1;
		sem_post(ph->is_dead);
	}
	sem_post(ph->speak);
}

t_philo	init_philo(t_ph *ph)
{
	t_philo	philo;

	philo.id = ph->current + 1;
	sem_post(ph->id);
	philo.nb_eat = 0;
	philo.state = EATING;
	gettimeofday(&(philo.last_eat), NULL);
	if (ph->nb == 1)
		phi_my_sleep(ph->t_die + 1);
	return (philo);
}

int	phi_is_dead(t_ph *ph)
{
	int	res;

	sem_wait(ph->is_dead);
	res = sem_wait(ph->dead);
	if (res == -1)
	{
		sem_post(ph->is_dead);
		return (1);
	}
	else
	{
		sem_post(ph->dead);
		sem_post(ph->is_dead);
		return (0);
	}
	
}

void	free_ph(t_ph *ph)
{
	sem_close(ph->id);
	sem_close(ph->dead);
	sem_close(ph->is_dead);
	sem_close(ph->speak);
	sem_close(ph->fork);
	sem_close(ph->check);
}
