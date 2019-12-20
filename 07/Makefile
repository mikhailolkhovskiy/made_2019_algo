CC=g++

all: build_13 build_14_2

build_13: 13.o 
	$(CC) -o 13.exe 13.o

13.o: 13.cpp
	$(CC) -c 13.cpp

build_14_2: 14_2.o 
	$(CC) -o 14_2.exe 14_2.o

14_2.o: 14_2.cpp
	$(CC) -c 7_4.cpp

clean:
	rm -rf *.o *.exe
