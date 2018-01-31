#ifndef FUNC

#define FUNC

#include "main.h"

int calcSum(int * array, int size)
{
  int i,rtrn = 0;
  for(i = 0; i < size; i++)
  {
    rtrn += array[i];
  }
  return rtrn;
}

#endif
