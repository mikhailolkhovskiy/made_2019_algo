CC=g++

all: build_2_4 build_1_2

build_1_2: 1_2.o 
	$(CC) -o 1_2.exe 1_2.o

1_2.o: 1_2.cpp
	$(CC) -c 1_2.cpp

build_2_4: 2_4.o 
	$(CC) -o 2_4.exe 2_4.o

2_4.o: 2_4.cpp
	$(CC) -c 2_4.cpp

clean:
	rm -rf *.o *.exe
