# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alidy <alidy@student.42lyon.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/01/30 17:41:29 by alidy             #+#    #+#              #
#    Updated: 2021/02/03 14:39:16 by alidy            ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

HEADER			=		include/philosophers.h

SRC_PATH		=		srcs

SRCS_NAME		=		philosophers.c 				\
						phi_utils.c 				\
						
OBJS			=		${SRCS:.c=.o}

NAME			=		philo_one

RM				=		rm -f

CC				=		gcc

FLAGS			=		-Wall -Wextra -Werror -pthread

SRCS			=		$(addprefix $(SRC_PATH)/,$(SRCS_NAME))

all:					NAME

%.o: %.c				$(HEADER)
	@echo "\033[0;32m[OK] \033[0m \033[0;33m Compiling:\033[0m" $<
	@$(CC) $(FLAGS) -I $(HEADER) -c $< -o $@

NAME:				$(OBJS) $(HEADER) 
	@echo "\x1b[36m\n[OK] \033[0m \x1b[35m Compiling Philosophers\033[0m\n"
	@$(CC) $(FLAGS) -I $(HEADER) -o $(NAME) $(SRCS)
	
clean:
	@echo "\x1b[36m[OK] \033[0m \x1b[31m Removing File object\033[0m"
	@$(RM) $(OBJS)

fclean:					clean
	@echo "\x1b[36m[OK] \033[0m \x1b[31m Removing All\033[0m"
	@$(RM) $(NAME)

re:                                fclean all