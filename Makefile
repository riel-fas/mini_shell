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
USER := $(shell whoami)
READLINE_PATH = /Users/$(USER)/.brew/opt/readline
READLINE_FLAGS = -I$(READLINE_PATH)/include
READLINE_LIBS = -L$(READLINE_PATH)/lib -lreadline

# Directories
SRC_DIR			=	src
INC_DIR			=	includes

# Source files
SRCS = \
	$(SRC_DIR)/minishell.c \
	$(SRC_DIR)/exec/cmd_path.c \
	$(SRC_DIR)/exec/cmd_run.c \
	$(SRC_DIR)/exec/cmd_run_2.c \
	$(SRC_DIR)/exec/cmd_run_3.c \
	$(SRC_DIR)/exec/exec_error.c \
	$(SRC_DIR)/exec/exec_utils.c \
	$(SRC_DIR)/exec/execution.c \
	$(SRC_DIR)/exec/execution_2.c \
	$(SRC_DIR)/exec/execution_3.c \
	$(SRC_DIR)/built/builtin.c \
	$(SRC_DIR)/built/cd_utils.c \
	$(SRC_DIR)/built/cd.c \
	$(SRC_DIR)/built/echo.c \
	$(SRC_DIR)/built/env.c \
	$(SRC_DIR)/built/exit.c \
	$(SRC_DIR)/built/export_utils.c \
	$(SRC_DIR)/built/export_utils_2.c \
	$(SRC_DIR)/built/export.c \
	$(SRC_DIR)/built/handle_builtin_redi_utils.c \
	$(SRC_DIR)/built/handle_builtin_redi.c \
	$(SRC_DIR)/built/pwd.c \
	$(SRC_DIR)/built/unset.c \
	$(SRC_DIR)/built/utils.c \
	$(SRC_DIR)/env/env_init.c \
	$(SRC_DIR)/lexer/checker_tok.c \
	$(SRC_DIR)/lexer/cmd_split.c \
	$(SRC_DIR)/lexer/handle_tokens.c \
	$(SRC_DIR)/lexer/tokeniser_utils_2.c \
	$(SRC_DIR)/lexer/tokeniser_utils.c \
	$(SRC_DIR)/lexer/tokeniser.c \
	$(SRC_DIR)/parser/expanding.c \
	$(SRC_DIR)/parser/parser_split.c \
	$(SRC_DIR)/parser/parser_utils.c \
	$(SRC_DIR)/parser/parser.c \
	$(SRC_DIR)/redirections/redirections.c \
	$(SRC_DIR)/signal/signal_handling_2.c \
	$(SRC_DIR)/signal/signal_handling.c \
	$(SRC_DIR)/signal/signal.c \
	$(SRC_DIR)/utils/free_utils.c \
	$(SRC_DIR)/utils/minishell_utils_2.c \
	$(SRC_DIR)/utils/minishell_utils_3.c \
	$(SRC_DIR)/utils/minishell_utils.c \
	$(SRC_DIR)/utils/nodes_utils.c \
	$(SRC_DIR)/utils/quotes_utils.c \
	$(SRC_DIR)/utils/get_next_line/get_next_line_utils.c \
	$(SRC_DIR)/utils/get_next_line/get_next_line.c \
	$(SRC_DIR)/heredoc/heredoc_exp.c \
	$(SRC_DIR)/heredoc/heredoc_utils_2.c \
	$(SRC_DIR)/heredoc/heredoc_utils.c \
	$(SRC_DIR)/heredoc/heredoc.c

OBJS			=	$(SRCS:.c=.o)
HEADER			=	$(INC_DIR)/minishell.h	$(INC_DIR)/get_next_line.h

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

# %.o : %.c $(HEADER)
# 	@printf "${GREEN}Compiling $<...${NC}\n"
# 	@$(CC) $(ALL_INCLUDES) -c $< -o $@

%.o : %.c $(HEADER)
	@printf "${GREEN}Compiling $<...${NC}\n"
	@$(CC) $(ALL_INCLUDES) -c $< -o $@

$(OBJS): | compile_msg
	@$(CC) $(ALL_INCLUDES) -c $(@:.o=.c) -o $@

compile_msg:
	@printf "${GREEN}Compiling source files...${NC}\n"


$(LIBFT_A):
	@printf "${CYAN}Compiling libft...${NC}\n"
	@$(MAKE) -C $(LIBFT_DIR) all --no-print-directory > /dev/null

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
