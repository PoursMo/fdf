CC = gcc
CFLAG = -Wall -Wextra -Werror -Ilibft -Imlx
LFLAG = -Llibft -lft -Lmlx -lmlx_Linux -lXext -lX11 -lm
NAME = fdf
SRCSDIR = srcs/
SRCS = $(SRCSDIR)fdf.c
OBJS = $(SRCS:.c=.o)
LIB = libft/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIB)
	$(CC) $(CFLAG) $< -o $@ $(LFLAG)

$(LIB):
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