# Makefile for Event Management System

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iheaders
TARGET = build/event-management

# Directories
SRC_DIR = src
HEADERS_DIR = headers
BUILD_DIR = build

# Source files
SOURCES = $(SRC_DIR)/main.cpp $(SRC_DIR)/admin.cpp $(SRC_DIR)/student.cpp $(SRC_DIR)/user.cpp $(SRC_DIR)/event.cpp $(SRC_DIR)/registration.cpp $(SRC_DIR)/utils.cpp
HEADERS = $(HEADERS_DIR)/admin.h $(HEADERS_DIR)/student.h $(HEADERS_DIR)/user.h $(HEADERS_DIR)/event.h $(HEADERS_DIR)/registration.h $(HEADERS_DIR)/utils.h
OBJECTS = $(BUILD_DIR)/main.o $(BUILD_DIR)/admin.o $(BUILD_DIR)/student.o $(BUILD_DIR)/user.o $(BUILD_DIR)/event.o $(BUILD_DIR)/registration.o $(BUILD_DIR)/utils.o

# Default target
all: $(TARGET)

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Link the executable
$(TARGET): $(BUILD_DIR) $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "✓ Build successful! Run with: ./$(TARGET)"

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "✓ Clean complete"

# Rebuild everything
rebuild: clean all

# Help
help:
	@echo "Available targets:"
	@echo "  make           - Build the project"
	@echo "  make clean     - Remove build artifacts"
	@echo "  make rebuild   - Clean and rebuild"
	@echo "  make help      - Show this help message"

.PHONY: all clean rebuild help
