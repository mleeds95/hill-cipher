# File: Makefile
# Author: Matthew Leeds
# Last Edit: 2015-01-20

CC=g++
CFLAGS=
OBJ=main.o matrix.o strassenMultiply.o

project1: $(OBJ)
	$(CC) $(CFLAGS) -o project1 $(OBJ)

main.o: main.cpp strassenMultiply.cpp
	$(CC) $(CFLAGS) -c main.cpp

matrix.o: matrix.cpp
	$(CC) $(CFLAGS) -c matrix.cpp

strassenMultiply.o: strassenMultiply.cpp matrix.cpp
	$(CC) $(CFLAGS) -c strassenMultiply.cpp

clean:
	rm project1 *.o
