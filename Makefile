NAME=pipex
CC=gcc
CFLAGS=-Wall -Wextra -Werror
SRC=pipex.c \
	args/check_arguments.c \
	logger/log_error.c \
	file/check_file_access.c \
	exec/execute.c
OBJ=$(SRC:.c=.o)

LIBFT_DIR=libft
LIBFT=$(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -L$(LIBFT_DIR) -lft

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	make -C $(LIBFT_DIR) clean
	rm -f $(OBJ)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all