
NAME = webserver
FILES = server/main.cpp  server/tools/printForDebug.cpp \
		server/tools/tools.cpp  server/tools/tools1.cpp server/serverSetup/Server.cpp \
 		server/serverSetup/sendResponse.cpp server/serverSetup/eventLoopt.cpp \
		server/serverSetup/readRequest.cpp server/parsing/fillCreateServers.cpp \
		server/parsing/fromatVariableCheck.cpp \
		server/parsing/parsing.cpp server/parsing/parsingTocknes.cpp \
		server/Client.cpp \
		request/RequestHandler.cpp request/RequestParser.cpp request/MultiPart.cpp request/cookies.cpp\
		Response/Response.cpp Response/cgi.cpp server/parsing/bracketValidtion.cpp \
		server/parsing/tocknesCases.cpp server/parsing/tocknizeValue.cpp \
	
OBJ = $(FILES:.cpp=.o) 
CC = c++ -I./include # -fsanitize=address,undefined -g # -std=c++98 
FLAGS = #-Werror -Wextra -Wall  -std=c++98

%.o:%.cpp webserv.hpp
	$(CC) $(FLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CC)  $(FLAGS) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)
fclean: clean	
	$(RM) $(NAME)

re: fclean all 