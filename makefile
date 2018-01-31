all:
	mpicc -Wall main.c main.h Comm_Line.c functions.c -o main -lm
