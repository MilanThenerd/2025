files = $(wildcard *.cpp)
sources = $(filter-out main.cpp, $(files))

GLAD_SRC = ../glad/src/glad.c
INCLUDES = -I../glad/include


main: $(files)
	g++ -g $(sources) main.cpp $(GLAD_SRC) $(INCLUDES) -lglfw -pthread -ldl -lGL -o main

clean:
	rm -f *.o main

run:
	./main

all:
	make clean
	make
	make run
