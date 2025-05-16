# # Colors
# GREEN = \033[0;32m
# YELLOW = \033[0;33m
# CYAN = \033[0;36m
# NC = \033[0m # No Color

# NAME			=	minishell

# CC				=	cc

# CFLAGS			=	-Wall -Wextra -Werror

# SRCS			=	src/main.c

# OBJS			=	${SRCS:.c=.o}

# HEADER			=	minishell.h

# # Readline support
# READLINE_PATH	=	/Users/riel-fas/.brew/opt/readline
# READLINE_FLAGS	=	-I$(READLINE_PATH)/include
# READLINE_LIBS	=	-L$(READLINE_PATH)/lib -lreadline

# all	:	banner $(NAME)

# banner:
# 	@echo "\033[0;31m███╗   ███╗██╗███╗   ██╗██╗▄▄███▄▄·██╗  ██╗███████╗██╗     ██╗     \033[0m"
# 	@echo "\033[0;31m████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     \033[0m"
# 	@echo "\033[0;31m██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     \033[0m"
# 	@echo "\033[0;31m██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     \033[0m"
# 	@echo "\033[0;31m██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗\033[0m"
# 	@echo "\033[0;31m╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚═▀▀▀══╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\033[0m"
# 	@echo "\033[0;31m▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\033[0m"
# 	@echo "\033[0;31m█                                                               █\033[0m"
# 	@echo "\033[0;31m█        🩸  WELCOME TO THE CURSED MINISHELL  🩸                █\033[0m"
# 	@echo "\033[0;31m█                                                               █\033[0m"
# 	@echo "\033[0;31m█    \"Every command you type echoes in the void...\"             █\033[0m"
# 	@echo "\033[0;31m█    \"Segfaults... are not bugs — they're warnings...\"          █\033[0m"
# 	@echo "\033[0;31m█    \"Fork, and you may awaken what sleeps in the pipe...\"      █\033[0m"
# 	@echo "\033[0;31m█                                                               █\033[0m"
# 	@echo "\033[0;31m▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀\033[0m"
# 	@echo ""
# 	@echo "$(YELLOW)Date: $(shell date '+%Y-%m-%d %H:%M:%S') UTC$(NC)"
# 	@echo "$(YELLOW)Haunted by: $(USER), roubelka 👻$(NC)"
# 	@echo ""

# $(NAME)	:	$(OBJS)
# 	@echo "$(GREEN)Linking objects to create $(NAME)...$(NC)"
# 	@$(CC) $(CFLAGS) $(READLINE_FLAGS) $(OBJS) -o $(NAME) $(READLINE_LIBS)
# 	@echo "$(GREEN)$(NAME) successfully compiled!$(NC)"

# %.o : %.c	$(HEADER)
# 	@echo "$(GREEN)Compiling $<...$(NC)"
# 	@$(CC) $(CFLAGS) $(READLINE_FLAGS) -c $< -o $@

# clean	:
# 	@echo "$(YELLOW)Removing object files...$(NC)"
# 	@rm -rf $(OBJS)

# fclean	:	clean
# 	@echo "$(YELLOW)Removing executable...$(NC)"
# 	@rm -rf $(NAME)

# re	:	fclean all

# .PHONY	:	all clean fclean re banner




# Colors
RED		= \033[0;31m
GREEN	= \033[0;32m
YELLOW	= \033[0;33m
CYAN	= \033[0;36m
NC		= \033[0m # No Color

NAME			=	minishell
CC				=	cc
CFLAGS			=	-Wall -Wextra -Werror

# Libft
LIBFT_DIR		=	./libft
LIBFT_A			=	$(LIBFT_DIR)/libft.a
LIBFT_INC		=	-I$(LIBFT_DIR)

# Readline support
READLINE_PATH	=	/Users/roubelka/.brew/opt/readline
READLINE_FLAGS	=	-I$(READLINE_PATH)/include
READLINE_LIBS	=	-L$(READLINE_PATH)/lib -lreadline

# Files
SRCS			=	src/main.c
OBJS			=	${SRCS:.c=.o}
HEADER			=	minishell.h

# Final compile flags
ALL_INCLUDES	=	$(CFLAGS) $(READLINE_FLAGS) $(LIBFT_INC)

all	:	banner $(NAME)

banner:
	@printf "${RED}███╗   ███╗██╗███╗   ██╗██╗▄▄███▄▄·██╗  ██╗███████╗██╗     ██╗     ${NC}\n"
	@printf "${RED}████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     ${NC}\n"
	@printf "${RED}██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     ${NC}\n"
	@printf "${RED}██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     ${NC}\n"
	@printf "${RED}██║ ╚═╝ ██║██║██║ �█████║██║███████║██║  ██║███████╗███████╗███████╗${NC}\n"
	@printf "${RED}╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚═▀▀▀══╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝${NC}\n"
	@printf "${RED}▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄${NC}\n"
	@printf "${RED}█                                                               █${NC}\n"
	@printf "${RED}█        🩸  WELCOME TO THE CURSED MINISHELL  🩸                █${NC}\n"
	@printf "${RED}█                                                               █${NC}\n"
	@printf "${RED}█    \"Every command you type echoes in the void...\"             █${NC}\n"
	@printf "${RED}█    \"Segfaults... are not bugs — they're warnings...\"          █${NC}\n"
	@printf "${RED}█    \"Fork, and you may awaken what sleeps in the pipe...\"      █${NC}\n"
	@printf "${RED}█                                                               █${NC}\n"
	@printf "${RED}▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀${NC}\n"
	@printf "\n"
	@printf "${YELLOW}Date: $(shell date '+%Y-%m-%d %H:%M:%S') UTC${NC}\n"
	@printf "${YELLOW}Haunted by: $(USER), roubelka 👻${NC}\n"
	@printf "\n"

$(NAME)	:	$(LIBFT_A) $(OBJS)
	@printf "${GREEN}Linking objects to create $(NAME)...${NC}\n"
	@$(CC) $(ALL_INCLUDES) $(OBJS) $(LIBFT_A) -o $(NAME) $(READLINE_LIBS)
	@printf "${GREEN}$(NAME) successfully compiled!${NC}\n"

%.o : %.c $(HEADER)
	@printf "${GREEN}Compiling $<...${NC}\n"
	@$(CC) $(ALL_INCLUDES) -c $< -o $@

$(LIBFT_A):
	@printf "${CYAN}Compiling libft...${NC}\n"
	@$(MAKE) -C $(LIBFT_DIR) bonus --no-print-directory > /dev/null

clean	:
	@printf "${YELLOW}Removing object files...${NC}\n"
	@rm -rf $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@printf "${YELLOW}Removing executable...${NC}\n"
	@rm -rf $(NAME)
	@printf "${YELLOW}Cleaning Libft (bonus)...${NC}\n"
	@$(MAKE) -s -C libft fclean

re	:	fclean all

.PHONY	:	all clean fclean re banner
