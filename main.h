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
#define QUAN 1

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
