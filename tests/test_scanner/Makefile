
CC=gcc
CFLAGS=-g -Wall -Wextra -std=c11

BIN=basic_test
SRC=basic_test.c ../../scanner.c ../../stack.c ../../dynamic_string.c
OBJ=$(patsubst %.c,%.o,$(SRC))

.PHONY: all
all: touch $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f $(BIN) $(OBJ)

.PHONY: touch
touch: $(SRC)
	touch $(SRC)

.PHONY: test
test: all
	./$(BIN)
debug: $(BIN)
	gdb -tui ./$(BIN)
