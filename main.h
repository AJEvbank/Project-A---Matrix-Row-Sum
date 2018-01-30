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
#define COLS 1000
#define SEED 1
#define GRAN 2500

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

#endif
