# Source directory
SRC_DIR = src
# Build directory
BUILD_DIR = build
# Include directory for header files
INCLUDE_DIR = include

# Target executable
TARGET = $(BUILD_DIR)/one-shot-tests.exe

# Compiler
CC = gcc
# Compiler flags: show all warnings, include directory, and pkg-config flags for GTK and SDL
CFLAGS = -Wall -I$(INCLUDE_DIR) `pkg-config --cflags gtk+-3.0 sdl2`
# Linker flags: pkg-config libraries for GTK and SDL
LDFLAGS = `pkg-config --libs gtk+-3.0 sdl2`

# Source files
SRCS = $(SRC_DIR)/main.c

# Object files, mapping source files to build directory
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Default target
all: $(TARGET)

# Link object files to create the target executable
$(TARGET): $(OBJS) | $(BUILD_DIR)
    $(CC) -o $@ $^ $(LDFLAGS)

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
    $(CC) $(CFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
    mkdir -p $(BUILD_DIR)

# Clean build directory
clean:
    rm -rf $(BUILD_DIR)

# Run the target executable
run: $(TARGET)
    ./$(TARGET)

# Phony targets
.PHONY: all clean run
