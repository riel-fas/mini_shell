NAME	=	libft.a

CC	= cc

CCFLAGS	= -Wall -Wextra -Werror

SRC = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstclear_bonus.c

SRCB = ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstclear_bonus.c

OBJ = $(SRC:.c=.o)

OBJB = $(SRCB:.c=.o)

AR = ar rcs


all	:	${NAME}

$(NAME)	:	$(OBJ)
	$(AR) $(NAME) $(OBJ)

%.o : %.c libft.h
	$(CC) $(CCFLAGS) -c $< -o $@

bonus :	$(OBJB)
	$(AR) $(NAME) $(OBJB)

clean	:
		rm	-rf	$(OBJ) $(OBJB)

fclean	:	clean
		rm -rf $(NAME)

re	:	fclean	all

.PHONY : all bonus clean fclean re
