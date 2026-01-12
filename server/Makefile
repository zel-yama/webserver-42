
NAME = server
FILES = main.cpp  tools/printForDebug.cpp \
		tools/tools.cpp  tools/tools1.cpp serverSetup/Server.cpp \
 		serverSetup/sendResponse.cpp serverSetup/eventLoopt.cpp \
		serverSetup/readRequest.cpp parsing/fillCreateServers.cpp \
		parsing/fromatVariableCheck.cpp \
		parsing/parsing.cpp parsing/parsingTocknes.cpp \
		Client.cpp

	
OBJ = $(FILES:.cpp=.o) 
CC = c++ -I./include  -fsanitize=address,undefined -g -std=c++98 
FLAGS = #-Werror -Wextra -Wall  -std=c++98

%.o:%.cpp ../webserv.hpp
	$(CC) $(FLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CC)  $(FLAGS) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)
fclean: clean	
	$(RM) $(NAME)

re: fclean all 