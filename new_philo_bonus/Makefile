# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/28 16:29:51 by lpellier          #+#    #+#              #
#    Updated: 2021/12/02 11:52:09 by lpellier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CFLAGS = -Wall -Werror -Wextra -pedantic

CC = clang

SRCS = \
		main.c \
		init.c \
		utils.c \
		destroy.c \
		philo_does.c \
		philo_utils.c

OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME) -lpthread

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all