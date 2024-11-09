CC = gcc
CFLAG = -Wall -Wextra -Werror -Ilibft -Iminilibx-linux
LFLAG = -Llibft -lft -Lminilibx-linux -lmlx_Linux -lXext -lX11 -lm
NAME = fdf
SRCSDIR = srcs/
SRCS = $(SRCSDIR)fdf.c
OBJS = $(SRCS:.c=.o)
LIBS =	libft/libft.a \
		minilibx-linux/mlx_Linux.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBS)
	$(CC) $(CFLAG) $< -o $@ $(LFLAG)

$(LIBS):
	make -C libft
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