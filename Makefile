CC = g++
CFLAGS = -Wall -Wextra -Werror -MMD -MP
LFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

BIN_DIR	= bin/
SRC_DIR	= src/
OBJ_DIR	= obj/
INC_DIR	= include

NAME	= cub3dpp
SRCS	= $(notdir $(wildcard $(SRC_DIR)*.cpp))
OBJS	= $(SRCS:.cpp=.o)
DEPS	= $(OBJS:.o=.d)

OBJFILES = $(addprefix $(OBJ_DIR), $(OBJS))
DEPFILES = $(addprefix $(OBJ_DIR), $(DEPS))

all: $(NAME)

$(NAME): $(OBJFILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -I $(INC_DIR) $(OBJFILES) $(LFLAGS) -o $(BIN_DIR)$(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I $(INC_DIR) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -rf $(BIN_DIR) build

re: fclean all

# run: all
# 	./$(BIN_DIR)$(NAME)

run:
	@mkdir -p build
	cmake -B build && cmake --build build
	./build/cub3dpp

.PHONY: all clean fclean re run

-include $(DEPFILES)
