CC = g++
CFLAGS = -std=c++11 -Wall -g
LDFLAGS = -lldap -lldap_r -llber

all: 
	./server

server: server.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f server