# Paths to directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# The name of the executable file
TARGET = $(BUILD_DIR)/one-shot-tests.exe

# Compiler and flags
CC = gcc
CFLAGS = -Wall -I$(INCLUDE_DIR) `pkg-config --cflags gtk+-3.0 sdl2 SDL2_ttf`
LDFLAGS = `pkg-config --libs gtk+-3.0 sdl2 SDL2_ttf`

# Source files
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/question.c

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Default command
all: $(TARGET)

# Compilation of the target executable file
$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compiling object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Cleaning the compilation files
clean:
	rm -rf $(BUILD_DIR)

# Running the program
run: $(TARGET)
	$(TARGET)

.PHONY: all clean run
