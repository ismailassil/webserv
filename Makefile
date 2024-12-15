# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iassil <iassil@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/04 21:44:18 by iassil            #+#    #+#              #
#    Updated: 2024/12/15 19:52:39 by iassil           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CPP				=	c++
CPP				+=	-Wall -Wextra -Werror -std=c++98
RM				=	rm -f
NAME			=	webserv
SRC_HR_H		=	bits/Request.hpp
FLD_NAME		=	._object_files

##### SOURCE FILES #######################################################################
SRC_FILES	=	main.cpp
RQS_FILES	=	Request.cpp

##########################################################################################

SRC_SRC		=	$(addprefix src/,$(SRC_FILES))
RQS_SRC		=	$(addprefix request/,$(RQS_FILES))

SRC_OBJ		=	$(addprefix $(FLD_NAME)/,$(SRC_SRC:.cpp=.o))
RQS_OBJ		=	$(addprefix $(FLD_NAME)/,$(RQS_SRC:.cpp=.o))

OBJ 		=	$(SRC_OBJ) $(RQS_OBJ)

########### Goal Target
all: $(NAME)

run: all
	./$(NAME)

$(FLD_NAME)/src/%.o:./src/%.cpp $(SRC_HR_H)
	@mkdir -p $(dir $@)
	@$(CPP) -c $< -o $@

$(FLD_NAME)/request/%.o:./request/%.cpp $(SRC_HR_H)
	@mkdir -p $(dir $@)
	@$(CPP) -c $< -o $@

######################################################

$(NAME): $(OBJ)
	@echo "$(YELLOW)[ ~ ] Compilation of the Objects files...$(RESET)"
	@$(CPP) $^ -o $@
	@echo "$(GREEN)[ ✓ ] Executable file Compiled Successfully!$(RESET)"

clean:
	@echo "$(YELLOW)[ ~ ] Removing Object files $(RESET)"
	@$(RM) $(OBJ)
	@$(RM) -rf $(FLD_NAME)
	@echo "$(GREEN)[ ✓ ] Object files removed successfully!$(RESET)"
	
fclean: clean
	@echo "$(YELLOW)[ ~ ] Removing Executable...$(RESET)"
	@$(RM) $(NAME)
	@echo "$(GREEN)[ ✓ ] Executable files removed successfully!$(RESET)"

re: fclean all

.PHONY: all fclean clean re

########## DFine ANSI escape codes for colors
GREEN=	\033[1;32m
YELLOW=	\033[33m
RESET=	\033[0m		# No Color