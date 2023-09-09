CXX = g++
CXXFLAGS = -std=c++98 -Wextra -Werror -Wall -g3
INCLUDES = -I./includes
LIBS = -L/usr/lib/aarch64-linux-gnu/


# Source files
SRCS = src/Channel.cpp src/Client.cpp src/Server.cpp main.cpp
SRCS += src/Commands/Command.cpp src/Commands/Join.cpp src/Commands/Mode.cpp src/Commands/Names.cpp
SRCS += src/Commands/Nick.cpp src/Commands/Part.cpp src/Commands/Pass.cpp src/Commands/Ping.cpp
SRCS += src/Commands/Privmsg.cpp src/Commands/Quit.cpp src/Commands/Topic.cpp src/Commands/User.cpp
SRCS += src/Commands/Who.cpp src/Commands/Whois.cpp src/Commands/invite.cpp src/Commands/kick.cpp
SRCS += src/Commands/list.cpp

HEADERS = includes/Channel.hpp includes/Client.hpp includes/Command.hpp includes/Server.hpp

# Object files
OBJ_DIR = objets
OBJS := $(patsubst src/%.cpp,objets/%.o,$(SRCS))
SUBDIRS := $(sort $(dir $(OBJS)))

# Executable name
EXECUTABLE = ircserv

# Color codes for terminal output
COLOR_RED = \033[0;31m
COLOR_GREEN = \033[0;32m
COLOR_RESET = \033[0m

.PHONY: all clean fclean re

all: $(OBJ_DIR) $(HEADERS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	@echo "$(COLOR_GREEN)Linking$(COLOR_RESET) $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

objets/%.o: src/%.cpp
	@echo "$(COLOR_GREEN)Compiling$(COLOR_RESET) $<"
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -c $< -o $@

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