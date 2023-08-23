# Compiler settings
CXX = g++
CXXFLAGS = -std=c++98 -Wextra -Werror -Wall -g3
INCLUDES = -I.

# Source files
SRCS = main.cpp Client.cpp Server.cpp   # List all your source files here

# Header files
HDRS = $(wildcard *.hpp)

# Object files
OBJ_DIR = objets
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

# Executable name
EXECUTABLE = beautiful_server

# Color codes for terminal output
COLOR_RED = \033[0;31m
COLOR_GREEN = \033[0;32m
COLOR_RESET = \033[0m

# Emoji codes for terminal output
FLAME_EMOJI = "U+\xF0\x9F\x9A\x80"

.PHONY: all clean fclean re

all: $(OBJ_DIR) $(EXECUTABLE)
	@echo "$(COLOR_GREEN)$(FLAME_EMOJI) Compilation successful! Server is ready! $(FLAME_EMOJI)$(COLOR_RESET)"

$(EXECUTABLE): $(OBJS)
	@echo "$(COLOR_GREEN)Linking$(COLOR_RESET) $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(OBJS)

$(OBJ_DIR)/%.o: %.cpp $(HDRS)
	@echo "$(COLOR_GREEN)Compiling$(COLOR_RESET) $<"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@echo "$(COLOR_RED)Cleaning$(COLOR_RESET)"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "$(COLOR_RED)Force Cleaning$(COLOR_RESET)"
	@rm -f $(EXECUTABLE)

re: fclean all