# Directories for source files, build files, and include files
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Target executable name
TARGET = $(BUILD_DIR)/one-shot-tests

# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -I$(INCLUDE_DIR) `pkg-config --cflags gtk+-3.0`
LDFLAGS = `pkg-config --libs gtk+-3.0`

# Source files and object files
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/question.c $(SRC_DIR)/config.c
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Default target to build the executable
all: $(TARGET)

# Rule to link object files into the target executable
$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) -o $@ $^ $(LDFLAGS)

# Rule to compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Rule to clean the build directory
clean:
	rm -rf $(BUILD_DIR)

# Rule to run the target executable
run: $(TARGET)
	./$(TARGET)

# Phony targets to avoid conflicts with files of the same name
.PHONY: all clean run