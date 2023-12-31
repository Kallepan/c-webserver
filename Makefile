# Specify the compiler
CXX = g++

# Specify the target
TARGET = build/main

# Specify the source files
SRC = src/*.cc

# Specify the compiler flags
CXXFLAGS = -Wall -std=c++11 -I include

# Default target
all: $(TARGET)

# Rule to build the target
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Rule to clean the build files
clean:
	rm -f $(TARGET)