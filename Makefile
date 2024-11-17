CC = gcc
CFLAG = -Wall -Wextra -Werror -Ilibft -Iminilibx-linux
LFLAG = -Llibft -lft -Lminilibx-linux -lmlx_Linux -lXext -lX11 -lm
NAME = fdf
SRCSDIR = srcs/
SRCS =	$(SRCSDIR)fdf.c			\
		$(SRCSDIR)parse.c		\
		$(SRCSDIR)utils.c		\
		$(SRCSDIR)image.c		\
		$(SRCSDIR)color.c		\
		$(SRCSDIR)input.c		\
		$(SRCSDIR)projection.c	\
		$(SRCSDIR)setup.c		\
		$(SRCSDIR)minmax.c		\
		$(SRCSDIR)draw1.c		\
		$(SRCSDIR)draw2.c		\
		$(SRCSDIR)bresenheim.c	
OBJS = $(SRCS:.c=.o)
LIBFT = libft/libft.a
MLX = minilibx-linux/libmlx_Linux.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(MLX)
	$(CC) $(CFLAG) $(OBJS) -o $@ $(LFLAG)

$(MLX):
	cd minilibx-linux && ./configure
	cd ..

$(LIBFT):
	make -C libft

%.o: %.c
	$(CC) $(CFLAG) -c $< -o $@

clean:
	rm -f $(OBJS)
	make clean -C libft

fclean: clean
	rm -f $(NAME)
	make fclean -C libft

re: fclean all