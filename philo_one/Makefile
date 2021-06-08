# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/28 16:29:51 by lpellier          #+#    #+#              #
#    Updated: 2021/06/08 15:33:06 by lpellier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_one

CFLAGS = -Wall -Werror -Wextra -pedantic -g3

CC = clang

SRCS = main.c \
		utils.c \
		time_utils.c \
		philo_does.c \
		init_and_destroy.c

OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(SRCS) -o philo_one -lpthread

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)
	rm -rf philo_one

re : fclean all