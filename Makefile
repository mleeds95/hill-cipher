# File: Makefile
# Author: Matthew Leeds

CC=g++
CFLAGS=-g -Wall -std=c++11
OBJ=main.o Matrix.o

.PHONY: clean

hillcipher: $(OBJ)
	$(CC) $(CFLAGS) -o hillcipher $(OBJ)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

Matrix.o: Matrix.cpp
	$(CC) $(CFLAGS) -c Matrix.cpp

clean:
	-rm hillcipher *.o
