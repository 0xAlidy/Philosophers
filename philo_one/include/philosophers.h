/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alidy <alidy@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 17:45:44 by alidy             #+#    #+#             */
/*   Updated: 2021/02/04 10:10:19 by alidy            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdio.h> // enlever
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct	s_prm
{
	int			nb;
	int			sleep;
	int			eat;
	int			die;
	int			must_eat;
}				t_prm;

typedef struct	s_philo
{
	
}				t_philo;

int		ft_atoi(const char *str);
int		ft_strlen(char *str);
void	ft_write(char *str);

#endif