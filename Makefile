# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vini <vini@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/12 17:39:27 by vini              #+#    #+#              #
#    Updated: 2025/06/04 19:05:39 by vini             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
LIB = ar rcs
RM = rm -f
CC = c++
CFLAGS = -Wall -Werror -Wextra -std=c++98
# DEBUG = -g3 -fsanitize=address
SRC = main.cpp Server.cpp Client.cpp Channel.cpp join.cpp registration.cpp \
		mode.cpp invite.cpp kick.cpp part.cpp privmsg.cpp quit.cpp topic.cpp

OBJ_DIR = ./obj/
OBJ_FILES = $(SRC:.cpp=.o)
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_FILES))

HEADER = Server.hpp Client.hpp Channel.hpp

# COLOURS
DEF_COLOR = \033[0;39m
CUT = \033[K
RED = \033[31;1m
GREEN = \033[32;1m
YELLOW = \033[33;1m
BLUE = \033[34;1m
PURPLE = \033[35;1m
GREY = \033[30;1m
END = \033[0m

# RULES
all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o:%.cpp $(HEADER)
	@$(CC) $(CFLAGS) -c $< -o $@

# executable compiled
$(NAME): $(OBJ) $(HEADER)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "\n$(GREEN)executable compiled$(DEF_COLOR)-> $@\n"

# all .o files, obj directory and executable removed
clean:
	@$(RM) $(OBJ)
	@rm -rf $(OBJ_DIR)
	@echo "\n$(RED)All .o files and /obj directory removed$(DEF_COLOR)\n"

fclean: clean
	@$(RM) $(NAME)
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)Executable removed$(DEF_COLOR)\n"

re: fclean all

.PHONY: all clean fclean re