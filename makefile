CC = gcc

CFLAGS = -g -std=c++11 -lstdc++
 
all: compfs

compfs: main.cpp token.hpp scanner.hpp scanner.cpp parser.hpp parser.cpp semantics.hpp semantics.cpp generator.hpp generator.cpp
	$(CC) -o $@ $^ $(CFLAGS) 
 
clean:
	rm compfs 
	rm -r *.dSYM *.asm