NAME = server 

SRCS = main.cpp ConfigParser.cpp Handlers.cpp ServerManager.cpp
OBJS = $(SRCS:.cpp=.o)
RM = rm -rf 
CC = g++ 
CFLAGS = -Wall -Wextra -Werror -std=c++98

all : $(NAME)
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o:%.cpp ConfigParser.hpp ServerManager.hpp 
	$(CC) $(CFLAGS) -c $< -o $@

clean : 
	$(RM) $(OBJS)

fclean: clean 
	$(RM) $(NAME)

re: fclean all 

run: clean
	./$(NAME)
.PHONY: re all fclean clean 
