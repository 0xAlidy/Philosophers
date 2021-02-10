/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alidy <alidy@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 17:45:44 by alidy             #+#    #+#             */
/*   Updated: 2021/02/10 12:54:04 by alidy            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# define EATING 1
# define SLEEPING 2
# define THINKING 3
# define FORK 4
# define DIED 5

typedef struct		s_philo 
{
	int				id;		
	int				nb_eat;
	int				state;
	struct timeval	last_eat;
}					t_philo;


typedef struct		s_ph
{
	int				nb;
	int				t_sleep;
	int				t_eat;
	int				t_die;
	int				must_eat;
	int				is_dead;
	int				current;
	pthread_mutex_t	*forks;
	pthread_mutex_t speak;
	pthread_mutex_t dead;
	long long		time;
}					t_ph;

int					ft_atoi(const char *str);
int					ft_strlen(char *str);
long long 			ft_timersub(struct timeval *s, t_ph *ph);

#endif