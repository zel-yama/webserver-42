NAME = request_test

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = main.cpp \
       Request.cpp \
       RequestParser.cpp \
       RequestHandler.cpp

OBJS = $(SRCS:.cpp=.o)

HEADERS = Request.hpp \
          RequestParser.hpp \
          RequestHandler.hpp

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re 