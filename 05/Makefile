CC=g++

all: build

run: build
	./main

Huffman.o: Huffman.cpp Huffman.h
	$(CC) -c Huffman.cpp
	
main.o: main.cpp Huffman.h
	$(CC) -c main.cpp

build: main.o Huffman.o 
	$(CC) -o main main.o Huffman.o

clean:
	rm -rf *.o main
