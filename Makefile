CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGET = simulator
SRC = simulator.c

TEST ?= tests/task1/minimal.bin
REF ?= $(TEST:.bin=.res)
OUTPUT = output.bin

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

test: $(TARGET)
	./$(TARGET) $(TEST) $(OUTPUT)

check: $(TARGET)
	@./$(TARGET) $(TEST) $(OUTPUT) > /dev/null
	@cmp $(OUTPUT) $(REF) && echo "PASS: $(TEST)" || (echo "FAIL: $(TEST)"; exit 1)

tests: $(TARGET)
	./test_all.sh

clean:
	rm -f $(TARGET) $(OUTPUT)

.PHONY: all test check clean tests