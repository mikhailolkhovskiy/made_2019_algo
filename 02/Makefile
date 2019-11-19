CC=g++

all: build_3_2 build_4_1

build_3_2: 3_2.o 
	$(CC) -o 3_2.exe 3_2.o

3_2.o: 3_2.cpp
	$(CC) -c 3_2.cpp

build_4_1: 4_1.o 
	$(CC) -o 4_1.exe 4_1.o

4_1.o: 4_1.cpp
	$(CC) -c 4_1.cpp

clean:
	rm -rf *.o *.exe
