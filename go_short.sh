#! /bin/bash

clear
make

echo "************************************************************************************************************************************************"
time mpirun -np 4 ./main -gran 1 -seed 42 -print #-rows 46340 -cols 46340
