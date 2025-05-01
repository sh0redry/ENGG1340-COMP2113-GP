# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic-errors -I./src

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Source files
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# Target executable
TARGET = $(BIN_DIR)/game

# Create necessary directories
$(shell mkdir -p $(BUILD_DIR) $(BIN_DIR))

# Main target
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@

# Compile
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Generate dependencies
-include $(DEPS)

# Clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean
