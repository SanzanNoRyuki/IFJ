###############################
# Makefile for dynamic string #
# Author: Vojtěch Ulej	      #
###############################

# Compiler
CC = gcc
CFLAGS =-std=c11 -Wall -Wextra -g

SRC=dynamic_string_test.c ../../dynamic_string.c

.PHONY: all
all: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o dm_str_test
