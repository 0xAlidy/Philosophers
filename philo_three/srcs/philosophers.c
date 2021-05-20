/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alidy <alidy@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 13:44:52 by alidy             #+#    #+#             */
/*   Updated: 2021/05/20 12:40:57 by alidy            ###   ########lyon.fr   */
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
	sem_wait(ph->check);
	if (ph->nb_fork - 2 >= 0)
		ph->nb_fork -= 2;
	else
	{
		sem_post(ph->check);
		return ;
	}
	sem_post(ph->check);
	sem_wait(ph->fork);
	sem_wait(ph->fork);
	print_state(ph, philo->id, FORK);
	print_state(ph, philo->id, FORK);
	print_state(ph, philo->id, EATING);
	gettimeofday(&(philo->last_eat), NULL);
	phi_my_sleep(ph->t_eat);
	sem_post(ph->fork);
	sem_post(ph->fork);
	sem_wait(ph->check);
	ph->nb_fork += 2;
	sem_post(ph->check);
	philo->nb_eat += 1;
	philo->state = SLEEPING;
}

int		start_routine(t_ph *ph)
{
	t_philo	philo;

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
			phi_eat(ph, &philo);
		else
			phi_sleep(ph, &philo);
	}
	if ((ph->must_eat != -1 && philo.nb_eat >= ph->must_eat))
		return (0);
	return (1);
}

void	philosophers(t_ph *ph)
{
	int				i;
	pid_t			*pid;
	pid_t			current_pid;
	struct timeval	temp;
	int				status;

	i = 0;
	gettimeofday(&temp, NULL);
	ph->time = ft_time(&temp);
	pid = malloc(sizeof(pid_t) * ph->nb);
	status = 0;
	if (!pid)
		return ;
	while (i < ph->nb)
	{
		sem_wait(ph->id);
		ph->current = i;
		current_pid = fork();
		if (current_pid == -1)
			exit(1);
		else if (current_pid == 0)
		{
			pid[i] = current_pid;
			exit(start_routine(ph));
		}
		++i;
	}
	i = 0;
	while (i < ph->nb)
	{
		waitpid(0, &status, 0);
		++i;
		if (status != 0)
			break;
	}
	free(pid);
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
