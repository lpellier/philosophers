# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lpellier <lpellier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/28 16:29:51 by lpellier          #+#    #+#              #
#    Updated: 2021/05/28 16:31:16 by lpellier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philosophers

CFLAGS = -Wall -Werror -Wextra -pedantic -g3

CC = clang

SRCS = srcs/main.c

OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
        $(CC) $(CFLAGS) $(SRCS) -o philosophers

clean :
        rm -rf $(OBJS)

fclean : clean
        rm -rf $(NAME)
        rm -rf philosophers

re : fclean all