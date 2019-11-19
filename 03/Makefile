CC=g++

all: build_6_4 build_7_4

build_6_4: 6_4.o 
	$(CC) -o 6_4.exe 6_4.o

6_4.o: 6_4.cpp
	$(CC) -c 6_4.cpp

build_7_4: 7_4.o 
	$(CC) -o 7_4.exe 7_4.o

7_4.o: 7_4.cpp
	$(CC) -c 7_4.cpp

clean:
	rm -rf *.o *.exe
