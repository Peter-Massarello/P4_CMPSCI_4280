CC = gcc

CFLAGS = -g -std=c++11 -lstdc++
 
all: statSem

statSem: main.cpp token.hpp scanner.hpp scanner.cpp parser.hpp parser.cpp semantics.hpp semantics.cpp
	$(CC) -o $@ $^ $(CFLAGS) 
 
clean:
	rm statSem 
	rm -r *.dSYM