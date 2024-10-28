NAME = ircserv

SRC_PATH = src/

SRC		=	main.cpp						\
			src/utils.cpp						\
			src/server.cpp						\
			src/server_getset.cpp				\
			src/server_poll.cpp					\
			src/server_exception.cpp			\
			src/read_data.cpp					\
			src/user.cpp						\
			src/channel.cpp						\
			src/capLs.cpp						\
			src/pass.cpp						\
			src/nick.cpp						\
			src/user_cmd.cpp					\
			src/join.cpp						\
			src/privmsg.cpp						\
			src/ping.cpp						\
			src/mode.cpp						\
			src/topic.cpp						\
			src/kick.cpp						\
			src/quit.cpp						\
			src/invite.cpp						\
			src/part.cpp						\

INC = inc
OBJ_DIR = obj
OBJS = $(patsubst $(SRC_PATH)%.cpp,$(OBJ_DIR)/%.o,$(SRC))

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g3

TOTAL_FILES := $(words $(SRC))
CURRENT_FILE = 0
PERCENTAGE = 0

GREEN = \033[32m
RED = \033[31m
YELLOW = \033[33m
BOLD = \e[1m
UNBOLD = \e[0m
RESET = \033[0m

define update_progress
    $(eval CURRENT_FILE=$(shell echo $$(($(CURRENT_FILE)+1))))
    $(eval PERCENTAGE=$(shell echo $$(($(CURRENT_FILE)*100/$(TOTAL_FILES)))))
	@printf "$(GREEN)\rCompiling [%-20s] %d%% $(RESET)" "####################" "$(PERCENTAGE)"
endef

all : $(NAME)

$(NAME) : $(OBJS)
	@$(CXX) $(CXXFLAGS) $^ -I $(INC) -o $@
	@echo "\n $(BOLD)$(NAME)$(UNBOLD) is built  🪄$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_PATH)%.cpp | $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)
	$(call update_progress)
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(YELLOW)cleaning  🧽$(RESET)"

fclean: clean
	@rm -rf $(NAME)
	@echo "$(RED)full clean  🧹$(RESET)"

re: clean all

.PHONY : all clean re
