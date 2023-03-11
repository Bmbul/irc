NAME = irc

SRCS = $(wildcard *.cpp)

OBJS = $(patsubst %.cpp, %.o, $(SRCS))

HEADERS = $(wildcard *.hpp)
PRE_HEADERS = $(patsubst %.hpp, %.hpp.gch, $(HEADERS))

CFLAGS = -Wall -Wextra -Werror -std=c++98

CC = c++

RM = rm -rf

all: $(PRE_HEADERS) $(NAME)

%.hpp.gch: %.hpp
	@$(CC) $(CFLAGS) $< -o $@

%.o: %.cpp $(PRE_HEADERS)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $? -o $(NAME)

clean:
	@$(RM) $(OBJS)
	@$(RM) $(PRE_HEADERS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all


test:
	@echo $(OBJS)
	@echo $(PRE_HEADERS)

.PHONY: all clean fclean re