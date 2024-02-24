#############################################
#      Makefile for IFJ stack test          #
#      Author: Vojtěch Ulej (xulejv00)      #
#############################################

# Compiler
CC = gcc
CFLAGS =-std=iso9899:2011 -Wall -Wextra -g 

# Project
BIN1 = stack_test
BIN2 = p_stack_test
# Files
SRC = *.c ../../stack.c
INC = *.h ../../stack.h
OBJ = ../../stack.o $(BIN1).o  


.PHONY: pack all clean debug is_it_ok

all: $(BIN1)
	
# Packs project.
pack: clean


# Deletes unnecessary files.
clean:
	rm -f $(OBJ) $(BIN1)


# Compilation
$(BIN1): $(OBJ)
	$(CC) $(CFLAGS) -g $^ -o $@

../../stack.o: ../../stack.c
	$(CC) $(CFLAGS) -g -c $^ -o $@
