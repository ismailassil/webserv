# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iassil <iassil@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/04 21:44:18 by iassil            #+#    #+#              #
#    Updated: 2024/12/18 10:55:03 by iassil           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CPP				=	c++
CPP				+=	-Wall -Wextra -Werror -std=c++98
CPP				+=	-fsanitize=address -g
RM				=	rm -f
NAME			=	webserv
HEADER			=	bits/HTTPMethods.hpp		bits/RequestBuilder.hpp			bits/RequestParser.hpp		\
					bits/bits.hpp
PARSE_HEADER	=	bits/parse/BodyParser.hpp	bits/parse/HeaderParser.hpp		bits/parse/RequestLineParser.hpp

FLD_NAME		=	._object_files

##### SOURCE FILES #######################################################################
SRC_FILES	=	main.cpp
RQS_FILES	=	RequestParser.cpp	RequestBuilder.cpp
PRS_FILES	=	HeaderParser.cpp	RequestLineParser.cpp	BodyParser.cpp

##########################################################################################

SRC_SRC		=	$(addprefix srcs/,$(SRC_FILES))
RQS_SRC		=	$(addprefix request/,$(RQS_FILES))
PRS_SRC		=	$(addprefix request/parse/,$(PRS_FILES))

SRC_OBJ		=	$(addprefix $(FLD_NAME)/,$(SRC_SRC:.cpp=.o))
RQS_OBJ		=	$(addprefix $(FLD_NAME)/,$(RQS_SRC:.cpp=.o))
PRS_OBJ		=	$(addprefix $(FLD_NAME)/,$(PRS_SRC:.cpp=.o))

OBJ 		=	$(SRC_OBJ) $(RQS_OBJ) $(PRS_OBJ)

########### Goal Target
all: $(NAME)

run: all
	@./$(NAME)

$(FLD_NAME)/srcs/%.o: ./srcs/%.cpp $(HEADER)
	@mkdir -p $(dir $@)
	@$(CPP) -c $< -o $@

$(FLD_NAME)/request/%.o: ./request/%.cpp $(HEADER)
	@mkdir -p $(dir $@)
	@$(CPP) -c $< -o $@

$(FLD_NAME)/request/parse/%.o: ./request/parse/%.cpp $(PARSE_HEADER)
	@mkdir -p $(dir $@)
	@$(CPP) -c $< -o $@

######################################################
$(NAME): $(OBJ)
	@echo "$(YELLOW)[ ~ ] Compilation of the Objects files...$(RESET)"
	@$(RM) Request.txt
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

########## Define ANSI escape codes for colors
GREEN=	\033[1;32m
YELLOW=	\033[33m
RESET=	\033[0m