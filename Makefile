#############################################
#      Makefile for IFJ project             #
#      Author: Vojtěch Ulej (xulejv00)      #
#############################################

# Compiler
CC = gcc
CFLAGS =-std=c11 -Wall -Wextra -g

# Directories
TMPDIR = tests/tmp
# Project
BIN = IFJ19Compiler

# Files
SRC = $(shell echo *.c)
INC = $(shell echo *.h)
OBJ = $(patsubst %.c,%.o,$(SRC))
OK_SCRIPT = tests/is_it_ok.sh


ARCH = xulejv00.tbz2
PACKFILES = $(SRC) $(INC) Makefile rozdeleni dokumentace.pdf

.PHONY: all 
all: touch debug

# Packs project.
.PHONY: pack
pack: clean $(ARCH)

$(ARCH): $(PACKFILES)
	tar -acvf $@ $^

# Deletes unnecessary files.
.PHONY: clean
clean: clean_obj clean_is_ok
	rm -f $(BIN) xulejv00.tbz2 vgcore.*

.PHONY: clean
clean_obj:
	rm -f $(OBJ)

# Compilation for debugging
.PHONY: debug
debug: $(OBJ)
	$(CC) $(CFLAGS) -ggdb $^ -o $(BIN)

clean_compilation: touch debug
	rm -vf $(OBJ)


# Testing
.PHONY: is_it_ok
is_it_ok: $(ARCH) $(OK_SCRIPT)
	chmod +x $(OK_SCRIPT)
	$(OK_SCRIPT) $(ARCH) $(TMPDIR)

clean_is_ok:
	rm -rf $(TMPDIR)

touch:
	touch $(SRC) $(INC) Makefile

test_parser: tests/parser/parser_test.sh
	chmod u+x $^
	cd tests/parser && ./parser_test.sh
assembly: $(SRC)
	gcc -Os -S $(SRC)
.PHONY: test
test: all is_it_ok test_parser
	cd tests/dynamic_string && make && ./dm_str_test
	cd tests/stack && make && ./stack_test
	cd tests/test_scanner && make && ./basic_test

expression_parser.o: expression_parser.c 
	$(CC) $(CFLAGS) -Wimplicit-fallthrough=1 -c -o $@ $^


