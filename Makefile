# Compiler and flags
CC := clang
CFLAGS := -Wall -Wextra -Wpedantic -O2 -g -std=c17 -Isrc

# Test flags are fixed so make can track them deterministically
TEST_CFLAGS := $(CFLAGS) -DTESTS -Itests/

# Directories
BUILD_DIR := build
SRC_DIR := src
TEST_DIR := tests

# Source files
SRC := $(shell find $(SRC_DIR) -name "*.c")
# Exclude the main program from test-specific compilation to avoid multiple `main` symbols
SRC_NO_MAIN := $(filter-out $(SRC_DIR)/main.c, $(SRC))
TEST_SRC := $(wildcard $(TEST_DIR)/*.c)

# Object files
OBJ := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SRC:.c=.o))
TEST_OBJ := $(patsubst $(TEST_DIR)/%,$(BUILD_DIR)/tests/%, $(TEST_SRC:.c=.o))
TEST_SRC_OBJ := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/tests-src/%,$(SRC_NO_MAIN:.c=.o))

# Targets
TARGET := $(BUILD_DIR)/lint
TEST_TARGET := $(BUILD_DIR)/test

all: $(TARGET)

# Main program
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@

# Test program
$(TEST_TARGET): $(TEST_OBJ) $(TEST_SRC_OBJ)
	$(CC) $(TEST_SRC_OBJ) $(TEST_OBJ) -o $@

# Compile src/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test src copies (tests-src)
$(BUILD_DIR)/tests-src/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(TEST_CFLAGS) -c $< -o $@

# Compile test files (tests/)
$(BUILD_DIR)/tests/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(TEST_CFLAGS) -c $< -o $@

run-tests: $(TEST_TARGET)
	@echo running tests
	@./$(TEST_TARGET)

run: $(TARGET)
	@echo running $(TARGET)
	@./$(TARGET)

clean:
	@rm -rf $(BUILD_DIR)

.PHONY: all run run-tests clean
