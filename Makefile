NAME = minitalk
SERVER = server
CLIENT = client
LIB = libft/libft.a
INC = libft/libft.h
SERVER_SRCS = srcs/server/main.c
CLIENT_SRCS = srcs/client/main.c
SERVER_OBJS = $(SERVER_SRCS:.c=.o)
CLIENT_OBJS = $(CLIENT_SRCS:.c=.o)
CC = gcc
CFLAGS = -Wall -Wextra -Werror

$(NAME): $(SERVER) $(CLIENT) 

all: $(NAME)

$(SERVER): $(SERVER_OBJS) $(INC)
	make -C libft
	$(CC) $(CFLAGS) -o $(SERVER) $(SERVER_OBJS) $(LIB)

$(CLIENT): $(CLIENT_OBJS) $(INC) $(LIB)
	$(CC) $(CFLAGS) -o $(CLIENT) $(CLIENT_OBJS) $(LIB)

clean:
	rm -f $(CLIENT_OBJS) $(SERVER_OBJS)
	make clean -C libft

fclean: clean
	rm -f $(SERVER) $(CLIENT)
	make fclean -C libft

re: fclean all

.PHONY: all clean fclean re bonus
