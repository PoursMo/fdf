CC = gcc
CFLAG = -Wall -Wextra -Werror -Ilibft -Iminilibx-linux
LFLAG = -Llibft -lft -Lminilibx-linux -lmlx_Linux -lXext -lX11 -lm
NAME = fdf
SRCSDIR = srcs/
SRCS = $(SRCSDIR)fdf.c
OBJS = $(SRCS:.c=.o)
LIBFT = libft/libft.a
LIBMLX = minilibx-linux/mlx_Linux.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(LIBMLX)
	$(CC) $(CFLAG) $< -o $@ $(LFLAG)

$(LIBFT):
	make -C libft

$(LIBMLX):
	make -C minilibx-linux

%.o: %.c
	$(CC) $(CFLAG) -c $< -o $@

clean:
	rm -f $(OBJS)
	make clean -C libft

fclean: clean
	rm -f $(NAME)
	make fclean -C libft

re: fclean all