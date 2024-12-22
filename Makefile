# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iassil <iassil@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/04 21:44:18 by iassil            #+#    #+#              #
#    Updated: 2024/12/22 21:58:16 by iassil           ###   ########.fr        #
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
all: files $(NAME)

run: $(NAME) files art
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
	@$(CPP) $^ -o $@
	@echo "$(GREEN)[ ✓ ] Executable file Compiled Successfully!$(RESET)"

files:
	@$(RM) *.py _downloads/*

clean: files
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

###################################################
###################################################
TERM_WIDTH := $(shell tput cols)

PADDING := $(shell expr $(TERM_WIDTH) - 74)
ifeq ($(shell test $(PADDING) -lt 0; echo $$?),0)
    PADDING := 0
endif
PADDING := $(shell expr $(PADDING) / 2)
PAD := $(shell printf '%*s' $(PADDING) '')
###################################################
###################################################

art:
	@echo ""
	@echo ""
	@echo "$(PAD)$(LIGHT_RED) █     █░▓█████  ▄▄▄▄     ██████ ▓█████  ██▀███   ██▒   █▓ $(RESET)"
	@echo "$(PAD)$(LIGHT_RED)▓█░ █ ░█░▓█   ▀ ▓█████▄ ▒██    ▒ ▓█   ▀ ▓██ ▒ ██▒▓██░   █▒ $(RESET)"
	@echo "$(PAD)$(LIGHT_RED)▒█░ █ ░█ ▒███   ▒██▒ ▄██░ ▓██▄   ▒███   ▓██ ░▄█ ▒ ▓██  █▒░ $(RESET)"
	@echo "$(PAD)$(RED)░█░ █ ░█ ▒▓█  ▄ ▒██░█▀    ▒   ██▒▒▓█  ▄ ▒██▀▀█▄    ▒██ █░░ $(RESET)"
	@echo "$(PAD)$(RED)░░██▒██▓ ░▒████▒░▓█  ▀█▓▒██████▒▒░▒████▒░██▓ ▒██▒   ▒▀█░   $(RESET)"
	@echo "$(PAD)$(RED)░ ▓░▒ ▒  ░░ ▒░ ░░▒▓███▀▒▒ ▒▓▒ ▒ ░░░ ▒░ ░░ ▒▓ ░▒▓░   ░ ▐░   $(RESET)"
	@echo "$(PAD)$(RED)  ▒ ░ ░   ░ ░  ░▒░▒   ░ ░ ░▒  ░ ░ ░ ░  ░  ░▒ ░ ▒░   ░ ░░   $(RESET)"
	@echo "$(PAD)$(DARK_RED)  ░   ░     ░    ░    ░ ░  ░  ░     ░     ░░   ░      ░░   $(RESET)"
	@echo "$(PAD)$(DARK_RED)    ░       ░  ░ ░            ░     ░  ░   ░           ░   $(RESET)"
	@echo "$(PAD)$(DARK_RED)                      ░                               ░   $(RESET)"
	@echo ""
	@echo ""

########## Define ANSI escape codes for colors
GREEN			=	\033[1;32m
YELLOW			=	\033[33m
LIGHT_RED		:=	\033[91m
RED				:=	\033[31m
DARK_RED		:=	\033[2;31m
RESET			=	\033[0m