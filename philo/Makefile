# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ivloisy <ivloisy@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/07 13:31:49 by ivloisy           #+#    #+#              #
#    Updated: 2021/11/14 15:26:54 by ivloisy          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc

FLAG = -Wall -Werror -Wextra -pthread

SRCDIR = sources

HDRDIR = includes

SRC = ./$(SRCDIR)/philosophers.c \
		./$(SRCDIR)/thread.c \
		./$(SRCDIR)/routine.c \
		./$(SRCDIR)/finish.c \
		./$(SRCDIR)/utils.c

HDR = ./$(HDRDIR)/philosophers.h

OBJDIR = objects

OBJ = $(OBJDIR)/*.o

all: $(NAME)

$(NAME): $(OBJ)
	@echo "\033[36m\n$(NAME) creation\t\t\t\t\t🛠\n"
	$(CC) $(FLAG) -o $@ $+
	@echo "\033[35m\n$(NAME) has been created with success !\t\t🌈\n"

$(OBJ): $(SRC) $(HDR)
	@echo "\033[32m\nSources files compilation\t\t\t⚙️\n"
	$(CC) $(FLAG) -c $(SRC)
	mkdir -p $(OBJDIR)
	mv *.o $(OBJDIR)

clean:
	@echo "\033[33m\nRemove objects files\t\t--->\t\t🗑\n"
	rm -rf $(OBJDIR)

fclean: clean
	@echo "\033[33m\nRemove $(NAME) file\t\t--->\t\t🗑\n"
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
