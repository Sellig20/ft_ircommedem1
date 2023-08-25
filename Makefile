# Compiler settings
# CXX = g++
# CXXFLAGS = -std=c++98 -Wextra -Werror -Wall -g3
# INCLUDES = -I.

# # Source files
# SRCS =	main.cpp \
# 		src/Client.cpp \
# 		src/Server.cpp \
# 		src/Commands/Command.cpp \
# 		 # List all your source files here

# # Header files
# HDRS = $(wildcard *.hpp)

# # Object files
# OBJ_DIR = objets
# OBJ_SRC = src
# OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

# # Executable name
# EXECUTABLE = beautiful_server

# # Color codes for terminal output
# COLOR_RED = \033[0;31m]
# COLOR_GREEN = \033[0;32m]
# COLOR_RESET = \033[0m]

# # Emoji codes for terminal output
# FLAME_EMOJI = "U+\xF0\x9F\x9A\x80"

# .PHONY: all clean fclean re

# all: $(OBJ_DIR) $(EXECUTABLE)
# 	@echo "$(COLOR_GREEN)$(FLAME_EMOJI) Compilation successful! Server is ready! $(FLAME_EMOJI)$(COLOR_RESET)"

# $(EXECUTABLE): $(OBJS)
# 	@echo "$(COLOR_GREEN)Linking$(COLOR_RESET) $@"
# 	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(OBJS)

# $(OBJ_DIR)/%.o: %.cpp $(HDRS) | $(OBJ_SRC)
# 	@echo "$(COLOR_GREEN)Compiling$(COLOR_RESET) $<"
# 	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# $(OBJ_DIR):
# 	@mkdir -p $(OBJ_DIR)

# $(OBJ_SRC):
# 	@mkdir -p $(OBJ_DIR)/src

# $(OBJ_CO):
# 	@mkdir -p $(OBJ_SRC)/Commands


# clean:
# 	@echo "$(COLOR_RED)Cleaning$(COLOR_RESET)"
# 	@rm -rf $(OBJ_DIR)

# fclean: clean
# 	@echo "$(COLOR_RED)Force Cleaning$(COLOR_RESET)"
# 	@rm -f $(EXECUTABLE)

# re: fclean all

CXX = g++
CXXFLAGS = -std=c++98 -Wextra -Werror -Wall -g3
INCLUDES = -I.

# Répertoires
SRC_DIR = src
OBJ_DIR = objets

# Fichiers source
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# Exclure les fichiers source de Commands
SRCS := $(filter-out $(SRC_DIR)/Commands/%.cpp, $(SRCS))

# Fichiers source de la sous-dossier Commands
CMD_SRCS = $(wildcard $(SRC_DIR)/Commands/*.cpp)

# Fichiers objets générés
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
CMD_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CMD_SRCS))

EXECUTABLE = beautiful_server

# Couleurs pour l'affichage
COLOR_GREEN = \033[0;32m
COLOR_RESET = \033[0m

.PHONY: all clean fclean re

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/Commands:
	@mkdir -p $@

$(OBJ_DIR)/src:
	@mkdir -p $@

all: $(OBJ_DIR) $(EXECUTABLE)

$(EXECUTABLE): $(OBJS) $(CMD_OBJS)
	@echo "$(COLOR_GREEN)Linking$(COLOR_RESET) $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "$(COLOR_GREEN)Compiling$(COLOR_RESET) $<"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/Commands/%.cpp | $(OBJ_DIR)/src
	@echo "$(COLOR_GREEN)Compiling$(COLOR_RESET) $<"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


clean:
	@echo "$(COLOR_GREEN)Cleaning$(COLOR_RESET)"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "$(COLOR_GREEN)Force Cleaning$(COLOR_RESET)"
	@rm -f $(EXECUTABLE)

re: fclean all
