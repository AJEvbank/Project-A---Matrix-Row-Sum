all:
	mpicc -Wall main.c main.h Comm_Line.c -o main -lm
