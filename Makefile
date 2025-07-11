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
READLINE_PATH	=	/Users/roubelka/.brew/opt/readline
# READLINE_PATH	=	/Users/riel-fas/.brew/opt/readline
READLINE_FLAGS	=	-I$(READLINE_PATH)/include
READLINE_LIBS	=	-L$(READLINE_PATH)/lib -lreadline

# Directories
SRC_DIR			=	src
INC_DIR			=	includes

# Source files
SRCS = $(SRC_DIR)/main.c \
		$(SRC_DIR)/env/copy_env.c \
		$(SRC_DIR)/env/env_list.c \
		$(SRC_DIR)/env/env_utils.c \
		$(SRC_DIR)/mini_shell_clean.c \
		$(SRC_DIR)/mini_shell_init.c \
		$(SRC_DIR)/signal/signal.c \
		$(SRC_DIR)/signal/heredoc_signal.c \
		$(SRC_DIR)/mini_shell_loop.c \
		$(SRC_DIR)/mini_shell_loop_utils.c \
		$(SRC_DIR)/mini_shell_input.c \
		$(SRC_DIR)/utils/mini_shell_utils.c \
		$(SRC_DIR)/lexer/lexer.c \
		$(SRC_DIR)/lexer/lexer_extraction.c \
		$(SRC_DIR)/lexer/lexer_extraction_2.c \
		$(SRC_DIR)/lexer/lexer_extraction_utils.c \
		$(SRC_DIR)/lexer/lexer_utils.c \
		$(SRC_DIR)/lexer/lexer_utils_2.c \
		$(SRC_DIR)/lexer/expansion.c \
		$(SRC_DIR)/lexer/expansion_utils.c \
		$(SRC_DIR)/lexer/expansion_utils_2.c \
		$(SRC_DIR)/parser/parser_utils.c \
		$(SRC_DIR)/parser/syntax_error.c \
		$(SRC_DIR)/parser/syntax_error_2.c \
		$(SRC_DIR)/parser/parser_main.c \
		$(SRC_DIR)/parser/parser_cmd.c \
		$(SRC_DIR)/parser/parser_redirection.c \
		$(SRC_DIR)/exec/executor_process.c \
		$(SRC_DIR)/exec/execution_cleanup.c \
		$(SRC_DIR)/exec/pipeline_exec.c \
		$(SRC_DIR)/exec/redirections.c \
		$(SRC_DIR)/exec/file_operations.c \
		$(SRC_DIR)/exec/heredoc_pipe.c \
		$(SRC_DIR)/exec/heredoc_main.c \
		$(SRC_DIR)/exec/heredoc_list.c \
		$(SRC_DIR)/exec/heredoc_exec.c \
		$(SRC_DIR)/exec/exec_utils.c \
		$(SRC_DIR)/exec/executor.c \
		$(SRC_DIR)/exec/cmd_utils.c \
		$(SRC_DIR)/exec/cmd_path.c \
		$(SRC_DIR)/exec/env_converter.c \
		$(SRC_DIR)/exec/exec_signals.c \
		$(SRC_DIR)/exec/heredoc_signals.c \
		$(SRC_DIR)/exec/signal_restoration.c \
		$(SRC_DIR)/exec/single_exec.c \
		$(SRC_DIR)/exec/single_exec_utils.c \
		$(SRC_DIR)/exec/child_exec.c \
		$(SRC_DIR)/exec/error_handling.c \
		$(SRC_DIR)/exec/error_detection.c \
		$(SRC_DIR)/exec/error_checks.c \
		$(SRC_DIR)/exec/builtin_resolver.c \
		$(SRC_DIR)/built/cd.c \
		$(SRC_DIR)/built/cd_utils.c \
		$(SRC_DIR)/built/cd_utils2.c \
		$(SRC_DIR)/built/colon.c \
		$(SRC_DIR)/built/echo.c \
		$(SRC_DIR)/built/env.c \
		$(SRC_DIR)/built/exit.c \
		$(SRC_DIR)/built/export.c \
		$(SRC_DIR)/built/export_utils.c \
		$(SRC_DIR)/built/pwd.c \
		$(SRC_DIR)/built/unset.c \
		$(SRC_DIR)/built/unset_utils.c \
		$(SRC_DIR)/utils/get_next_line.c \
		$(SRC_DIR)/utils/get_next_line_utils.c \
		$(SRC_DIR)/utils/heredoc_utils.c \
		$(SRC_DIR)/utils/memory_management.c \
		$(SRC_DIR)/utils/memory_management_2.c


OBJS			=	$(SRCS:.c=.o)
HEADER			=	$(INC_DIR)/minishell.h $(INC_DIR)/lexer.h

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

$(OBJS): | compile_msg
	@$(CC) $(ALL_INCLUDES) -c $(@:.o=.c) -o $@

compile_msg:
	@printf "${GREEN}Compiling source files...${NC}\n"

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
