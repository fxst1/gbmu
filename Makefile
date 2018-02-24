CC=g++
FLAGS=-Wall -Wextra -g -O2 -fPIC -L.
LIB=`pkg-config gtkmm-3.0 --cflags --libs` -lpthread

SRC := $(filter %.cpp, $(shell find src -type f))
OBJ = $(SRC:.cpp=.o)

ifeq ($(OS),Windows_NT)
	NAME := gbmu.exe
	DETECT_OS := Windows
else
	NAME := gbmu
	DETECT_OS := $(shell uname -s)
endif

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) $(FLAGS) -o $(NAME) -I inc/ -I etc/ $(MAIN) $(OBJ) $(LIB)

$(ASMLIB):
	rm -rf $(ASMLIB)
	nasm -g $(SYSTEM) -o src/cpu/asm_instr.o src/cpu/asm_instr.s
	ld -shared -o $(ASMLIB) src/cpu/asm_instr.o
	sudo cp	-f ./$(ASMLIB) /usr/lib
	sudo chmod 0755 /usr/lib/$(ASMLIB)
	sudo ldconfig

lib :
	rm -rf $(ASMLIB)
	nasm -g $(SYSTEM) -o src/cpu/asm_instr.o src/cpu/asm_instr.s
	ld -shared -o $(ASMLIB) src/cpu/asm_instr.o
	sudo cp	-f ./$(ASMLIB) /usr/lib
	sudo chmod 0755 /usr/lib/$(ASMLIB)
	sudo ldconfig

%.o: %.cpp
		$(CC) $(FLAGS) -I inc/ -I etc/ -o $@ -c $< $(LIB)

test: $(OBJ)
		$(CC) $(FLAGS) -o $(NAME) -I inc/ -I etc/ $(MAIN) $(OBJ) $(ASMLIB) $(LIB)

clean:
		@rm -vf $(OBJ) $(OBJASM)

fclean: clean
		@rm -vf $(NAME)

	
re: fclean all


compile: $(OBJ)
	$(CC) $(FLAGS) -o $(NAME) -I inc/ $(MAIN)  $(OBJ) -lpthread

download:

	ifeq ($(DETECT_OS),Linux)

		apt-get install libgtkmm-3.0-dev

	endif
	ifeq ($(DETECT_OS),Darwin)

		#ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)" < /dev/null 2> /dev/null
		brew install gtkmm

	endif

install:

	make

	ifeq ($(DETECT_OS),Linux)

		cp -fv ./gbmu.desktop /usr/share/applications/
		cp -fv ./gbmu.desktop ~/.local/share/applications/

	endif
	ifeq ($(DETECT_OS),Darwin)

	endif
