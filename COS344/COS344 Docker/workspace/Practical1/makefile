# Define the compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

# Define the source and object files
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

# Define the target executable
TARGET = main

# Default rule (build the target)
all: $(TARGET)

# Rule to create the target executable
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

# Rule to compile .cpp files into .o files
%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove compiled files
clean:
	rm -f $(OBJ) $(TARGET)

# Run the compiled program
run: $(TARGET)
	./$(TARGET)

# Phony targets (not associated with files)
.PHONY: all clean run
