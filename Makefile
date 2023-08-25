CXX = g++
CXXFLAGS = -std=c++98 -Wextra -Werror -Wall -g3
INCLUDES = -I./includes

# Source files
SRCS := $(shell find src -name "*.cpp")
SRCS += main.cpp

# Object files
OBJ_DIR = objets
OBJS := $(patsubst src/%.cpp,objets/%.o,$(SRCS))
SUBDIRS := $(sort $(dir $(OBJS)))

# Executable name
EXECUTABLE = beautiful_server

# Color codes for terminal output
COLOR_RED = \033[0;31m
COLOR_GREEN = \033[0;32m
COLOR_RESET = \033[0m

.PHONY: all clean fclean re

all: $(OBJ_DIR) $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	@echo "$(COLOR_GREEN)Linking$(COLOR_RESET) $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

objets/%.o: src/%.cpp
	@echo "$(COLOR_GREEN)Compiling$(COLOR_RESET) $<"
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

objets/main.o: main.cpp
	@echo "$(COLOR_GREEN)Compiling$(COLOR_RESET) $<"
	mkdir -p $(dir $@)
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