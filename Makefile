
HEADER = philo.h
NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror  -MMD -pthread -I ./

SRCS = philo.c routine.c time.c utils.c

OBJ = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

%.o : %.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

all:$(NAME)

-include $(DEPS)
$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(^) -o $(NAME)

clean:
	rm -f $(OBJ) $(DEPS)

fclean:	clean
	rm -f $(NAME)

re: fclean all

.PHONY: all fclean clean re
 
