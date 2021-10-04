# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/28 16:29:51 by lpellier          #+#    #+#              #
#    Updated: 2021/10/04 11:33:42 by lpellier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus

CFLAGS = -Wall -Werror -Wextra -pedantic

CC = clang

SRCS = main.c \
		utils.c \
		time_utils.c \
		philo_does.c \
		init_and_destroy.c \
		process_init.c

OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME) -lpthread

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all