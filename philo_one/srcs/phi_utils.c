/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alidy <alidy@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 14:37:48 by alidy             #+#    #+#             */
/*   Updated: 2021/02/10 12:51:49 by alidy            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

long long 	ft_time(struct timeval *s)
{
	long long sec;
	long long micro;
	long long time;

	sec = s->tv_sec;
	micro = s->tv_usec;
	time = sec * 0.001 + micro * 1000;
	return (time);
}

long long 	ft_timersub(struct timeval *s, t_ph *ph)
{
	struct timeval end;
	long long time;
	long long time2;
	
	gettimeofday(&end, NULL);
	time = ft_time(s);
	time2 = ft_time(&end);
	ph->time += time2;
	return (time - time2);
}

int		ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		++i;
	return (i);
}

int		ft_atoi(const char *str)
{
	int		i;
	int		neg;
	long	res;

	i = 0;
	neg = 1;
	res = 0;
	while ((str[i] > 8 && str[i] < 14) || str[i] == 32)
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
	{
		i++;
		neg = -1;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - 48;
		i++;
	}
	return (res * neg);
}
