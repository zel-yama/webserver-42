
NAME = server
FILES = main.cpp  Server.cpp eventLoopt.cpp 
RM  = rm -rf 
OBJ = $(FILES:.cpp=.o) 
CC = c++ -std=c++98 
FLAGS = #-Werror -Wextra -Wall  -std=c++98

%.o:%.cpp Server.hpp
	$(CC) $(FLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CC)  $(FLAGS) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)
fclean: clean	
	$(RM) $(NAME)

re: fclean all 