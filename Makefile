NAME = Webserv
CC = c++ -Wall -Wextra -Werror -fsanitize=address -g -std=c++98

SRC_DIR = src
MAIN = main.cpp
OBJ_DIR = obj

SRCFILES = $(wildcard $(SRC_DIR)/*.cpp)
HEADERS = $(wildcard $(SRC_DIR)/*.hpp)

OBJFILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCFILES))

all: $(NAME)

$(NAME): $(OBJFILES)
	$(CC) -o $(NAME) $(OBJFILES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
