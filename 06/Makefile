CC=g++

all: build_11_1 build_12

build_11_1: 11_1.o 
	$(CC) -o 11_1.exe 11_1.o

11_1.o: 11_1.cpp
	$(CC) -c 11_1.cpp

build_12: 12.o 
	$(CC) -o 12.exe 12.o

12.o: 12.cpp
	$(CC) -c 12.cpp

clean:
	rm -rf *.o *.exe
