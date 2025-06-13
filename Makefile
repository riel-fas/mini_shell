########################################RIAD\MAKEFILE\LINUX##############################################
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
LIBFT_LIB		=	-L$(LIBFT_DIR) -lft

# Readline support
READLINE_PATH	=	/home/linuxbrew/.linuxbrew/opt/readline
READLINE_FLAGS	=	-I$(READLINE_PATH)/include/readline
READLINE_LIBS	=	-L$(READLINE_PATH)/lib -lreadline

# Directories
SRC_DIR			=	src
INC_DIR			=	includes

# Source files
SRCS			=	$(SRC_DIR)/main.c \
					$(SRC_DIR)/env/copy_env.c \
					$(SRC_DIR)/env/env_list.c \
					$(SRC_DIR)/mini_shell_clean.c \
					$(SRC_DIR)/mini_shell_init.c \
					$(SRC_DIR)/signal/signal.c \
					$(SRC_DIR)/mini_shell_loop.c \
					$(SRC_DIR)/utils/mini_shell_utils.c \
					$(SRC_DIR)/lexer/lexer.c \
					$(SRC_DIR)/lexer/lexer_extraction.c \
					$(SRC_DIR)/lexer/lexer_utils.c \
					$(SRC_DIR)/lexer/lexer_utils_2.c \
					$(SRC_DIR)/parser/syntax_error.c \
					$(SRC_DIR)/parser/print_command.c \
					$(SRC_DIR)/parser/parser_token.c \



OBJS			=	$(SRCS:.c=.o)
HEADER			=	$(INC_DIR)/minishell.h

# Final compile flags
ALL_INCLUDES	=	$(CFLAGS) $(READLINE_FLAGS) $(LIBFT_INC) -I$(INC_DIR)

all	:	banner $(NAME)

banner:
	@printf "${RED}███╗   ███╗██╗███╗   ██╗██╗▄▄███▄▄·██╗  ██╗███████╗██╗     ██╗     ${NC}\n"
	@printf "${RED}████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     ${NC}\n"
	@printf "${RED}██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     ${NC}\n"
	@printf "${RED}██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     ${NC}\n"
	@printf "${RED}██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗${NC}\n"
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
	@printf "${YELLOW}Haunted by: riel-fas, roubelka 👻${NC}\n"
	@printf "\n"

$(NAME)	:	$(LIBFT_A) $(OBJS)
	@printf "${GREEN}Linking objects to create $(NAME)...${NC}\n"
	@$(CC) $(ALL_INCLUDES) $(OBJS) -o $(NAME) $(LIBFT_LIB) $(READLINE_LIBS)
	@printf "${GREEN}$(NAME) successfully compiled!${NC}\n"

%.o : %.c $(HEADER)
	@printf "${GREEN}Compiling $<...${NC}\n"
	@$(CC) $(ALL_INCLUDES) -c $< -o $@

$(LIBFT_A):
	@printf "${CYAN}Compiling libft...${NC}\n"
	@$(MAKE) -C $(LIBFT_DIR) all bonus --no-print-directory > /dev/null

clean	:
	@printf "${YELLOW}Removing object files...${NC}\n"
	@rm -rf $(OBJS)
	@printf "${YELLOW}Cleaning Libft...${NC}\n"
	@$(MAKE) -C $(LIBFT_DIR) clean --no-print-directory

fclean: clean
	@printf "${YELLOW}Removing executable...${NC}\n"
	@rm -rf $(NAME)
	@printf "${YELLOW}Cleaning Libft completely...${NC}\n"
	@$(MAKE) -C $(LIBFT_DIR) fclean --no-print-directory

re	:	fclean all

.PHONY	:	all clean fclean re banner



#################################################RIAD\MAKEFILE\MACOS##########################################################################

# # Colors
# RED		= \033[0;31m
# GREEN	= \033[0;32m
# YELLOW	= \033[0;33m
# CYAN	= \033[0;36m
# NC		= \033[0m # No Color

# NAME			=	minishell
# CC				=	cc
# CFLAGS			=	-Wall -Wextra -Werror

# # Libft
# LIBFT_DIR		=	./libft
# LIBFT_A			=	$(LIBFT_DIR)/libft.a
# LIBFT_INC		=	-I$(LIBFT_DIR)
# LIBFT_LIB		=	-L$(LIBFT_DIR) -lft

# # Readline support
# READLINE_PATH	=	/Users/roubelka/.brew/opt/readline
# READLINE_FLAGS	=	-I$(READLINE_PATH)/include
# READLINE_LIBS	=	-L$(READLINE_PATH)/lib -lreadline

# # Directories
# SRC_DIR			=	src
# INC_DIR			=	includes

# # Source files
# SRCS			=	$(SRC_DIR)/main.c \
# 					$(SRC_DIR)/env/copy_env.c \
# 					$(SRC_DIR)/env/env_list.c \
# 					$(SRC_DIR)/mini_shell_clean.c \
# 					$(SRC_DIR)/mini_shell_init.c \
# 					$(SRC_DIR)/signal/signal.c \
# 					$(SRC_DIR)/mini_shell_loop.c \
# 					$(SRC_DIR)/utils/mini_shell_utils.c \
# 					$(SRC_DIR)/lexer/lexer.c \
# 					$(SRC_DIR)/lexer/lexer_extraction.c \
# 					$(SRC_DIR)/lexer/lexer_utils.c \
# 					$(SRC_DIR)/lexer/lexer_utils_2.c \
# 					$(SRC_DIR)/parser/syntax_error.c \
# 					$(SRC_DIR)/parser/print_command.c \
# 					$(SRC_DIR)/parser/parser_token.c \



# OBJS			=	$(SRCS:.c=.o)
# HEADER			=	$(INC_DIR)/minishell.h $(INC_DIR)/lexer.h

# # Final compile flags
# ALL_INCLUDES	=	$(CFLAGS) $(READLINE_FLAGS) $(LIBFT_INC) -I$(INC_DIR)

# all	:	banner $(NAME)

# banner:
# 	@printf "${RED}███╗   ███╗██╗███╗   ██╗██╗▄▄███▄▄·██╗  ██╗███████╗██╗     ██╗     ${NC}\n"
# 	@printf "${RED}████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     ${NC}\n"
# 	@printf "${RED}██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     ${NC}\n"
# 	@printf "${RED}██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     ${NC}\n"
# 	@printf "${RED}██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗${NC}\n"
# 	@printf "${RED}╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚═▀▀▀══╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝${NC}\n"
# 	@printf "${RED}▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄${NC}\n"
# 	@printf "${RED}█                                                               █${NC}\n"
# 	@printf "${RED}█        🩸  WELCOME TO THE CURSED MINISHELL  🩸                █${NC}\n"
# 	@printf "${RED}█                                                               █${NC}\n"
# 	@printf "${RED}█    \"Every command you type echoes in the void...\"             █${NC}\n"
# 	@printf "${RED}█    \"Segfaults... are not bugs — they're warnings...\"          █${NC}\n"
# 	@printf "${RED}█    \"Fork, and you may awaken what sleeps in the pipe...\"      █${NC}\n"
# 	@printf "${RED}█                                                               █${NC}\n"
# 	@printf "${RED}▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀${NC}\n"
# 	@printf "\n"
# 	@printf "${YELLOW}Date: $(shell date '+%Y-%m-%d %H:%M:%S') UTC${NC}\n"
# 	@printf "${YELLOW}Haunted by: riel-fas, roubelka 👻${NC}\n"
# 	@printf "\n"

# $(NAME)	:	$(LIBFT_A) $(OBJS)
# 	@printf "${GREEN}Linking objects to create $(NAME)...${NC}\n"
# 	@$(CC) $(ALL_INCLUDES) $(OBJS) -o $(NAME) $(LIBFT_LIB) $(READLINE_LIBS)
# 	@printf "${GREEN}$(NAME) successfully compiled!${NC}\n"

# # %.o : %.c $(HEADER)
# # 	@printf "${GREEN}Compiling $<...${NC}\n"
# # 	@$(CC) $(ALL_INCLUDES) -c $< -o $@

# %.o : %.c $(HEADER)
# 	@printf "${GREEN}Compiling $<...${NC}\n"
# 	@$(CC) $(ALL_INCLUDES) -c $< -o $@

# $(OBJS): | compile_msg
# 	@$(CC) $(ALL_INCLUDES) -c $(@:.o=.c) -o $@

# compile_msg:
# 	@printf "${GREEN}Compiling source files...${NC}\n"


# $(LIBFT_A):
# 	@printf "${CYAN}Compiling libft...${NC}\n"
# 	@$(MAKE) -C $(LIBFT_DIR) all bonus --no-print-directory > /dev/null

# clean	:
# 	@printf "${YELLOW}Removing object files...${NC}\n"
# 	@rm -rf $(OBJS)
# 	@printf "${YELLOW}Cleaning Libft...${NC}\n"
# 	@$(MAKE) -C $(LIBFT_DIR) clean --no-print-directory

# fclean: clean
# 	@printf "${YELLOW}Removing executable...${NC}\n"
# 	@rm -rf $(NAME)
# 	@printf "${YELLOW}Cleaning Libft completely...${NC}\n"
# 	@$(MAKE) -C $(LIBFT_DIR) fclean --no-print-directory

# re	:	fclean all

# .PHONY	:	all clean fclean re banner




######################################################RACHID\MAKEFILE#######################################################################


# SRCS            =   src/main.c \
#                     src/env/copy_env.c \
#                     src/env/env_list.c \
#                     src/mini_shell_clean.c \
#                     src/mini_shell_init.c \
#                     src/signal/signal.c \
#                     src/mini_shell_loop.c \
#                     src/utils/mini_shell_utils.c \
#                     src/lexer/lexer.c \
#                     src/lexer/lexer_extraction.c \
#                     src/lexer/lexer_utils.c \
#                     src/lexer/lexer_utils_2.c
# CC = cc
# CFLAGS = -Wall -Wextra -Werror
# NAME = minishell

# OBJS = $(SRCS:.c=.o)

# LIBFT_DIR = ./libft
# LIBFT = $(LIBFT_DIR)/libft.a

# all: $(NAME)

# $(NAME): $(OBJS) $(LIBFT)
#     $(CC) $(CFLAGS) -L/Users/roubelka/.brew/opt/readline/lib -lreadline $(OBJS) $(LIBFT) -o $(NAME)

# $(LIBFT):
#     $(MAKE) -C $(LIBFT_DIR)

# %.o: %.c
#     $(CC) $(CFLAGS) -I/Users/roubelka/.brew/opt/readline/include -c $< -o $@

# clean:
#     rm -f $(OBJS)
#     $(MAKE) clean -C $(LIBFT_DIR)

# fclean: clean
#     rm -f $(NAME)
#     $(MAKE) fclean -C $(LIBFT_DIR)

# re: fclean all

# .PHONY: all bonus clean fclean re
