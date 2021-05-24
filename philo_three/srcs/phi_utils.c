/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alidy <alidy@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 08:35:30 by alidy             #+#    #+#             */
/*   Updated: 2021/05/24 14:53:41 by alidy            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	phi_sleep(t_ph *ph, t_philo *philo)
{
	print_state(ph, philo->id, SLEEPING);
	phi_my_sleep(ph, ph->t_sleep);
	print_state(ph, philo->id, THINKING);
	philo->state = EATING;
}

void	print_state(t_ph *ph, int id, int state)
{
	sem_wait(ph->speak);
	phi_is_dead(ph);
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
		sem_wait(ph->is_dead);
		printf("%-5ld %d died\n", ft_timer(ph->time), id);
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
		phi_my_sleep(ph, ph->t_die + 1);
	return (philo);
}

void	phi_is_dead(t_ph *ph)
{
	sem_wait(ph->is_dead);
	sem_post(ph->is_dead);
}

void	free_ph(t_ph *ph)
{
	sem_close(ph->id);
	sem_close(ph->is_dead);
	sem_close(ph->speak);
	sem_close(ph->fork);
}
