#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct	s_philo
{
	int				index;
	int				eat_times;
	int				left_fork;
	int				right_fork;
	pthread_mutex_t	*forks;
	struct timeval	last_meal;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
}				t_philo;

void	display_message(char *str, t_philo *philo)
{
	struct timeval	time;
	int				timestamp;

	gettimeofday(&time, NULL);
	timestamp = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	printf("%d %d %s\n", timestamp, philo->index + 1, str);
}

void	*monitor(void *var)
{
	t_philo			*philo;
	struct timeval	time;
	int				time_diff;

	philo = (t_philo *)var;
	while (1)
	{
		gettimeofday(&time, NULL);
		time_diff = (time.tv_sec * 1000 + time.tv_usec / 1000) - (philo->last_meal.tv_sec * 1000 + philo->last_meal.tv_usec / 1000);
		if (time_diff > philo->time_to_die)
		{
			display_message("died", philo);
			exit(0);
		}
		usleep(1000);
	}
}

void	*philosopher(void *var)
{
	t_philo			*philo;
	pthread_t		monitor_thread;

	philo = (t_philo *)var;
	pthread_create(&monitor_thread, NULL, &monitor, philo);
	pthread_detach(monitor_thread);
	while (1)
	{
		pthread_mutex_lock(&philo->forks[philo->left_fork]);
		display_message("has taken a fork", philo);
		pthread_mutex_lock(&philo->forks[philo->right_fork]);
		display_message("has taken a fork", philo);
		gettimeofday(&philo->last_meal, NULL);
		display_message("is eating", philo);
		usleep(philo->time_to_eat * 1000);
		pthread_mutex_unlock(&philo->forks[philo->right_fork]);
		pthread_mutex_unlock(&philo->forks[philo->left_fork]);
		display_message("is sleeping", philo);
		usleep(philo->time_to_sleep * 1000);
		display_message("is thinking", philo);
	}
}

int	main(int argc, char **argv)
{
	pthread_t		*philosophers;
	pthread_mutex_t	*forks;
	t_philo			*philos;
	int				i;

	if (argc < 5 || argc > 6)
		return (1);
	philosophers = malloc(sizeof(pthread_t) * atoi(argv[1]));
	forks = malloc(sizeof(pthread_mutex_t) * atoi(argv[1]));
	philos = malloc(sizeof(t_philo) * atoi(argv[1]));
	i = -1;
	while (++i < atoi(argv[1]))
	{
		pthread_mutex_init(&forks[i], NULL);
		philos[i] = (t_philo){i, 0, i, (i + 1) % atoi(argv[1]), forks, {0, 0}, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), argc == 6 ? atoi(argv[5]) : 0};
		pthread_create(&philosophers[i], NULL, &philosopher, &philos[i]);
	}
	i = -1;
	while (++i < atoi(argv[1]))
		pthread_join(philosophers[i], NULL);
	free(philosophers);
	free(forks);
	free(philos);
	return (0);
}
