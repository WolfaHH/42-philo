
#include "philo.h"

int	philo_initialisation(t_info *data)
{
	int	i;

	data->t_start = timestamp();
	i = -1;
	while (++i < data->n_philo)
	{
		data->philo[i].n = i + 1;
		data->philo[i].last_eat = 0;
		data->philo[i].fork_r = NULL;
		data->philo[i].info = data;
		data->philo[i].m_count = 0;
		pthread_mutex_init(&(data->philo[i].fork_l), NULL);
		if (i == data->n_philo - 1)
			data->philo[i].fork_r = &data->philo[0].fork_l;
		else
			data->philo[i].fork_r = &data->philo[i + 1].fork_l;
		if (pthread_create(&data->philo[i].thread, NULL, \
				&philo_life, &(data->philo[i])) != 0)
			return (-1);
	}
	i = -1;
	while (++i < data->n_philo)
		if (pthread_join(data->philo[i].thread, NULL) != 0)
			return (-1);
	return (0);
}

int	check_num(char **str)
{
	int	i;
	int	j;

	i = 1;
	while (str[i])
	{
		j = 0;
		while (str[i][j])
		{
			if (!ft_isdigit(str[i][j]))
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	variable_initialisation(t_info *data, char **av)
{
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->m_stop, NULL);
	pthread_mutex_init(&data->m_eat, NULL);
	pthread_mutex_init(&data->dead, NULL);
	data->stop = 0;
	data->philo = malloc(sizeof(t_philo) * data->n_philo);
	if (data->philo == NULL)
		return (2);
	if (check_num(av))
	{
		printf("Invalid Arguments\n");
		return (1);
	}
	data->philo_eat = 0;
	data->n_philo = ft_atoi(av[1]);
	data->t_die = ft_atoi(av[2]);
	data->t_eat = ft_atoi(av[3]);
	data->t_sleep = ft_atoi(av[4]);
	if (av[5])
		data->n_eat = ft_atoi(av[5]);
	if (av[5] && data->n_eat == 0)
		return (1);
	return (0);
}

void	superfree(t_info *data)
{
	int	i;

	i = -1;
	while (++i < data->n_philo)
	{
		pthread_mutex_destroy(&data->philo[i].fork_l);
		pthread_mutex_destroy(data->philo[i].fork_r);
	}
	free(data->philo);
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->m_stop);
	pthread_mutex_destroy(&data->m_eat);
	pthread_mutex_destroy(&data->dead);
}

int	main(int ac, char **av)
{
	t_info	data;

	if (ac != 5 && ac != 6)
		return (0);
	if (variable_initialisation(&data, av) == 1)
	{
		free(data.philo);
		return (0);
	}
	philo_initialisation(&data);
	superfree(&data);
}