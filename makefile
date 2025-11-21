
# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -O2 -std=c17 -Isrc
LDFLAGS := 

# Directories
BUILD_DIR := build
SRC_DIR := src
TEST_DIR := tests

# Main program
TARGET := $(BUILD_DIR)/lint
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

# Test program
TEST_TARGET := $(BUILD_DIR)/test
TEST_SRC := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ := $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%.o,$(TEST_SRC))

# Default rule
all: $(TARGET)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	@mkdir -p $@

# Link main program
$(TARGET): $(OBJ) | $(BUILD_DIR)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Link test program
$(TEST_TARGET): CFLAGS += -DTESTS -Itests/
$(TEST_TARGET): $(OBJ) $(TEST_OBJ) | $(BUILD_DIR)
	$(CC) $(OBJ) $(TEST_OBJ) -o $@ $(LDFLAGS) 

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test files
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Run main program
run: $(TARGET)
	@./$(TARGET)

# Run tests
tests: $(TEST_TARGET)
	@echo "running tests"
	@./$(TEST_TARGET)

# Clean build artifacts
clean:
	@rm -rf $(BUILD_DIR)

.PHONY: all clean run tests

