#ifndef MAIN_H

#define MAIN_H

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "getopt.h"


#define MCW MPI_COMM_WORLD
#define ROWS 10000
#define COLS 10000
#define SEED 1
#define GRAN 100
#define QUAN (int)(((double)rand())/((double)RAND_MAX) * 32)
#define PRINT 0
#define CHECKSUM 1
#define TIME 0
#define CORRECT 0

// (int)(((double)rand())/((double)RAND_MAX) * 32)
// u++
// Note: max size of int = +2147483647
// Note: min size of int = -2147483648

enum isNumStates {
	INITIAL,
	PLUSORMINUS,
	ZERO,
	NUMBER,
	DECIMAL,
	ERROR
};

void CommLineArgs(int argc,
                  char ** argv,
                  unsigned long *rows,
                  unsigned long *cols,
                  unsigned long *seed,
									unsigned long *gran
                );

int isNumber(const char * str);

int calcSum(int * array, int size);

#endif
