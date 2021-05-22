/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alidy <alidy@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 13:44:52 by alidy             #+#    #+#             */
/*   Updated: 2021/05/22 16:45:34 by alidy            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	phi_eat(t_ph *ph, t_philo *philo)
{
	sem_wait(ph->check);
	dprintf(1, "fork: %d\n",(int)ph->fork);
	if ((int)ph->fork - 2 >= 0)
	{
		sem_wait(ph->fork);
		sem_wait(ph->fork);
		//ph->nb_fork = ph->fork - 2;
		dprintf(1, "fork: %p\n", ph->fork--);
		sem_post(ph->check);
		print_state(ph, philo->id, FORK);
		print_state(ph, philo->id, FORK);
		print_state(ph, philo->id, EATING);
		gettimeofday(&(philo->last_eat), NULL);
		phi_my_sleep(ph, ph->t_eat);
		sem_post(ph->fork);
		sem_post(ph->fork);
		sem_wait(ph->check);
		ph->nb_fork += 2;
		sem_post(ph->check);
		philo->nb_eat += 1;
		philo->state = SLEEPING;
	}
	else
	{
		sem_post(ph->check);
		return ;
	}
}

int	start_routine(t_ph *ph)
{
	t_philo	philo;

	philo = init_philo(ph);
	while (!phi_is_dead(ph) && (ph->must_eat == -1
			|| philo.nb_eat < ph->must_eat))
	{
		usleep(10);
		
		if (ph->t_die < ft_timersub(&(philo.last_eat)))
		{
			phi_my_sleep(ph, ph->t_die - ft_timersub(&(philo.last_eat)));
			print_state(ph, philo.id, DIED);
		}
		else if (philo.state == EATING)
			phi_eat(ph, &philo);
		else
			phi_sleep(ph, &philo);
	}
	if (ph->must_eat != -1 && philo.nb_eat == ph->must_eat)
		return (0);
	return (1);
}

void	philosophers(t_ph *ph, int status)
{
	int				i;
	pid_t			current_pid;
	pid_t			*pid;
	struct timeval	temp;

	i = -1;
	pid = malloc(sizeof(pid_t) * ph->nb);
	if (!pid)
		exit(1);
	gettimeofday(&temp, NULL);
	ph->time = ft_time(&temp);
	while (++i < ph->nb)
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
	}
	i = -1;
	while (++i < ph->nb)
	{
		waitpid(-1, &status, 0);
		dprintf(1,"exit\n");
		if (status != 0)
			break ;
	}
	
	i = -1;
	while (++i < ph->nb)
		if (pid[i] != 0)
			kill(pid[i], SIGTERM);
	free(pid);
}

int	main(int argc, char **argv)
{
	t_ph	ph;
	int		status;

	status = 0;
	if (init_ph(&ph, argc, argv))
	{
		philosophers(&ph, status);
		free_ph(&ph);
	}
	return (0);
}
