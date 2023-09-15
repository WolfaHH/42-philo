
NAME = philo

CFLAGS = -Wall -Wextra -Werror

RM = rm -rf

SRCS = 	src/philo.c\
		src/philo2.c\
		src/philo3.c\
		src/fonctions.c\

$(NAME) :
	gcc $(CFLAGS) $(SRCS) -o $(NAME)

all : $(NAME)

fclean : clean
	$(RM) $(NAME)

clean :
	$(RM) $(NAME)

re : fclean all
