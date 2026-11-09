
CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGET = simulator
SRC = simulator.c

TEST ?= tests/task1/minimal.bin
OUTPUT = output.bin

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

test: $(TARGET)
	./$(TARGET) $(TEST) $(OUTPUT)

check: test
	cmp $(OUTPUT) $(REF)

clean:
	rm -f $(TARGET) $(OUTPUT)

.PHONY: all test check clean
