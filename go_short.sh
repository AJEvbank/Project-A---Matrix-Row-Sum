#! /bin/bash

clear
make

echo "************************************************************************************************************************************************"
time mpirun -np 4 ./main -rows 12 -cols 12 -gran 1
