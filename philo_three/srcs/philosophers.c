/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alidy <alidy@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 13:44:52 by alidy             #+#    #+#             */
/*   Updated: 2021/05/22 18:20:52 by alidy            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	phi_eat(t_ph *ph, t_philo *philo)
{
	sem_wait(ph->fork);
	sem_wait(ph->fork);
	print_state(ph, philo->id, FORK);
	print_state(ph, philo->id, FORK);
	print_state(ph, philo->id, EATING);
	gettimeofday(&(philo->last_eat), NULL);
	phi_my_sleep(ph, ph->t_eat);
	sem_post(ph->fork);
	sem_post(ph->fork);
	philo->nb_eat += 1;
	philo->state = SLEEPING;
}

void *check_die(void *philo)
{
	t_philo	*p;

	p = philo;
	while (1)
	{
		if (p->t_die < ft_timersub(&(p->last_eat)))
		{
			//ajouter semaphore pour parler
			//printf("%-5ld %d died\n", ft_timer(*(p->time)), p->id);
			sem_wait(*(p->is_dead));
			*((*p->dead)) = -1;
			sem_post(*(p->is_dead));
			exit(1);
		}
		usleep(10);
	}
	return (0);
}

int	start_routine(t_ph *ph)
{
	t_philo		philo;
	pthread_t	thread;

	philo = init_philo(ph);
	if (pthread_create(&thread, NULL, check_die, &philo))
		return (1);
	pthread_detach(thread);
	while (ph->must_eat == -1 || philo.nb_eat < ph->must_eat)
	{
		usleep(10);
		if (philo.state == EATING)
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
		dprintf(1,"ok\n");
		if (status != 0)
			break ;
	}
	i = -1;
	while (++i < ph->nb)
	{
		
		if (pid[i] != 0)
		{
			dprintf(1,"kill\n");
			kill(pid[i], SIGTERM);
		}
	}
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
