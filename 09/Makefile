CC=g++

all: build_18_1 build_19

build_18_1: 18_1.o 
	$(CC) -o 18_1.exe 18_1.o

18_1.o: 18_1.cpp
	$(CC) -c 18_1.cpp

build_19: 19.o 
	$(CC) -o 19.exe 19.o

19.o: 19.cpp
	$(CC) -c 19.cpp

clean:
	rm -rf *.o *.exe
