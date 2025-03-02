# main:
# 	g++ -g -std=c++11 main.cpp -o main

# run:
# 	./main

# clean:
# 	rm main -f

# Compiler and flags
CXX = g++
CXXFLAGS = -g -Wall -std=c++11
TARGET = main

# Source files
SRCS = main.cpp

# Default target
all: $(TARGET)

# Build the target
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Run the program with Valgrind
valgrind: $(TARGET)
	valgrind --leak-check=full --track-origins=yes ./$(TARGET)

# Clean up build files
clean:
	rm -f $(TARGET)

# Rebuild the project (clean and build)
rebuild: clean all

# Phony targets (not files)
.PHONY: all run valgrind clean rebuild