# Specify the compiler
CXX = g++

# Specify the target
TARGET = build/server

# Specify the source files
SRC = src/server.cc

# Specify the compiler flags
CXXFLAGS = -Wall -std=c++11

# Default target
all: $(TARGET)

# Rule to build the target
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Rule to clean the build files
clean:
	rm -f $(TARGET)