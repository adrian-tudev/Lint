# Compiler and flags
CC := clang 
CFLAGS := -Wall -Wextra -Wpedantic -O2 -std=c17 -Isrc
LDFLAGS := 

# Project
TARGET := build/lint
SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c,build/%.o,$(SRC))

# Default rule
all: build $(TARGET)

# Create build directory if it doesn't exist
build:
	mkdir -p build

# Link target in build/
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Compile each source
build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

# Run program silently
run: $(TARGET)
	@./$(TARGET)

# Clean build artifacts silently
clean:
	@rm -rf build

.PHONY: all clean run build

