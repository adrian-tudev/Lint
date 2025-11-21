# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -O2 -std=c17 -Isrc
LDFLAGS :=

# Directories
BUILD_DIR := build
SRC_DIR := src
TEST_DIR := tests

# Collect all source files recursively
SRC := $(shell find $(SRC_DIR) -name "*.c")
OBJ := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SRC:.c=.o))

# Collect all test files
TEST_SRC := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ := $(patsubst $(TEST_DIR)/%, $(BUILD_DIR)/tests/%, $(TEST_SRC:.c=.o))

# Build test-specific copies of src objects (compiled with -DTESTS)
TEST_SRC_OBJ := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/tests-src/%,$(SRC:.c=.o))

# Binaries
TARGET := $(BUILD_DIR)/lint
TEST_TARGET := $(BUILD_DIR)/test

# Default rule
all: $(TARGET)

# Ensure build directory structure exists automatically
$(BUILD_DIR)/%/:
	@mkdir -p $@

# Link main program
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Build test binary (source + test objects)
build-test: CFLAGS += -DTESTS -Itests/
build-test: $(TEST_TARGET)

$(TEST_TARGET): CFLAGS += -DTESTS -Itests/
$(TEST_TARGET): $(TEST_SRC_OBJ) $(TEST_OBJ)
	$(CC) $(TEST_SRC_OBJ) $(TEST_OBJ) -o $@ $(LDFLAGS)

# Compile any .c file in src/ recursively
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test-specific copies of src/*.c into `build/tests-src/` (built with -DTESTS)
$(BUILD_DIR)/tests-src/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test files into a separate tests subdirectory to avoid name collisions
$(BUILD_DIR)/tests/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Run tests (build first if needed)
run-tests: $(TEST_TARGET)
	@echo "running tests"
	@./$(TEST_TARGET)

clean:
	@rm -rf $(BUILD_DIR)

# Run main program
run: $(TARGET)
	@ echo "running $(TARGET)"
	@./$(TARGET)

.PHONY: all clean build-test run-tests

