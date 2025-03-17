CXX				=	clang++
CXXFLAGS		=	-Wall -Wextra -Werror -fsanitize=address -g -std=c++98
RM				=	rm -f
NAME			=	webserv

REQUEST_HEADER	=	request/RequestBuilder.hpp	request/RequestParser.hpp
BITS_HEADER		=	srcs/bits.hpp srcs/WebServer.hpp
PARSE_HEADER	=	request/parse/BodyParser.hpp	request/parse/HeaderParser.hpp		request/parse/RequestLineParser.hpp
BODY_HEADER		=	request/parse/parseBody/BoundaryParser.hpp request/parse/parseBody/ChunkParser.hpp
CONFIG_HEADER	=	config/ConfigParser.hpp config/Location.hpp
SERVER_HEADER	=	server/Server.hpp

HEADER			=	$(REQUEST_HEADER) $(BITS_HEADER) $(PARSE_HEADER)	\
					$(BODY_HEADER) $(CONFIG_HEADER) $(SERVER_HEADER)
					
OBJ_DIR			=	.object_files

##### SOURCE FILES #######################################################################
SRC_FILES	=	main.cpp			bits.cpp			WebServer.cpp
SRV_FILES	=	Server.cpp
CONF_FILES	=	ConfigParser.cpp	Location.cpp
RQS_FILES	=	RequestParser.cpp	RequestBuilder.cpp
PRS_FILES	=	HeaderParser.cpp	RequestLineParser.cpp	BodyParser.cpp
BDY_FILES	=	BoundaryParser.cpp	ChunkParser.cpp

##########################################################################################

SRC_DIR		=	$(addprefix srcs/,$(SRC_FILES))
SRV_DIR		=	$(addprefix server/,$(SRV_FILES))
CONF_DIR	=	$(addprefix config/,$(CONF_FILES))
RQS_DIR		=	$(addprefix request/,$(RQS_FILES))
PRS_DIR		=	$(addprefix request/parse/,$(PRS_FILES))
BDY_DIR		=	$(addprefix request/parse/parseBody/,$(BDY_FILES))

# Object files for source files
SRC_OBJ		=	$(addprefix $(OBJ_DIR)/,$(SRC_DIR:.cpp=.o))
# Object files for server files
SRV_OBJ		=	$(addprefix $(OBJ_DIR)/,$(SRV_DIR:.cpp=.o))
# Object files for config files
CONF_OBJ	=	$(addprefix $(OBJ_DIR)/,$(CONF_DIR:.cpp=.o))
# Object files for request files
RQS_OBJ		=	$(addprefix $(OBJ_DIR)/,$(RQS_DIR:.cpp=.o))
# Object files for parse files
PRS_OBJ		=	$(addprefix $(OBJ_DIR)/,$(PRS_DIR:.cpp=.o))
# Object files for body files
BDY_OBJ		=	$(addprefix $(OBJ_DIR)/,$(BDY_DIR:.cpp=.o))

OBJ = $(SRC_OBJ) $(SRV_OBJ) $(CONF_OBJ) $(RQS_OBJ) $(PRS_OBJ) $(BDY_OBJ)

########### Goal Target
all: $(NAME)

run: $(NAME) art
	@./$(NAME)

$(OBJ_DIR)/srcs/%.o: ./srcs/%.cpp $(HEADER)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/server/%.o: ./server/%.cpp $(HEADER)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/config/%.o: ./config/%.cpp $(HEADER)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/request/%.o: ./request/%.cpp $(HEADER)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/request/parse/%.o: ./request/parse/%.cpp $(PARSE_HEADER)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/request/parse/parseBody/%.o: ./request/parse/parseBody/%.cpp $(PARSE_HEADER)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

######################################################
$(NAME): $(OBJ)
	@echo "$(YELLOW)[ ~ ] Compilation of the Objects files...$(RESET)"
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "$(GREEN)[ ✓ ] Executable file Compiled Successfully!$(RESET)"

clean:
	@echo "$(YELLOW)[ ~ ] Removing Object files $(RESET)"
	@$(RM) $(OBJ)
	@$(RM) -rf $(OBJ_DIR)
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
GREEN			:=	\033[1;32m
YELLOW			:=	\033[33m
LIGHT_RED		:=	\033[91m
RED				:=	\033[31m
DARK_RED		:=	\033[2;31m
RESET			:=	\033[0m
