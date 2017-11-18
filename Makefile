
NAME=gbmu

CC=g++
FLAGS=-Wall -Wextra -g -O2

SRC := $(filter %.cpp, $(shell find src -type f))
OBJ = $(SRC:.cpp=.o)

#MAIN = examples/expertTest.cpp


all: $(NAME)

$(NAME): $(OBJ)
		$(CC) $(FLAGS) -o $(NAME) -I inc/ $(MAIN)  $(OBJ) -lpthread

%.o: %.cpp
		$(CC) $(FLAGS) -I inc/ -o $@ -c $<

test: $(OBJ)
		$(CC) $(FLAGS) -o $(NAME) -I inc/ $(MAIN)  $(OBJ) -lpthread

clean:
		@rm -vf $(OBJ)

fclean: clean
		@rm -vf $(NAME)

install:
	

re: fclean all

compile: $(OBJ)
		$(CC) $(FLAGS) -o $(NAME) -I inc/ $(MAIN)  $(OBJ) -lpthread


#SDL
#apt-get install libsdl2-dev libsd2-image-dev libsdl2-ttf-dev
