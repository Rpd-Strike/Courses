PROGRAM_NAME = sortari

GCC_PARAMS = -std=c++11 -g -Wextra -Wall -fsanitize=address,undefined,signed-integer-overflow

INC=inc/
INC_PARAMS = $(foreach d, $(INC), -I$d)

LIB=
LIB_PARAMS = $(foreach d, $(LIB), -L$d)

SRC=main.cpp src/*.cpp src/*/*.cpp

default: main

main:  
	g++ $(GCC_PARAMS) $(INC_PARAMS) $(LIB_PARAMS) $(SRC) -o $(PROGRAM_NAME).o

clean:
	rm $(PROGRAM_NAME).o