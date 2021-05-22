/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_times.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alidy <alidy@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 08:24:20 by alidy             #+#    #+#             */
/*   Updated: 2021/05/22 17:26:45 by alidy            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

long	ft_time(struct timeval *s)
{
	return (s->tv_sec * 1000 + s->tv_usec / 1000);
}

long	ft_timersub(struct timeval *s)
{
	struct timeval	end;
	long			time;
	long			time2;

	gettimeofday(&end, NULL);
	time = ft_time(s);
	time2 = ft_time(&end);
	return (time2 - time);
}

long	ft_timer(long time)
{
	struct timeval	end;
	long			time2;

	gettimeofday(&end, NULL);
	time2 = ft_time(&end);
	return (time2 - time);
}

long	ft_have_time(struct timeval *s, int time)
{
	return (ft_timersub(s) + time);
}

void	phi_my_sleep(t_ph *ph, int time)
{
	int				i;
	struct timeval	start;

	i = 1;
	(void)ph;
	gettimeofday(&start, NULL);
	while (i == 1)
	{
		usleep(10);
		if (ft_timersub(&start) >= time)
			i = 0;
	}
}