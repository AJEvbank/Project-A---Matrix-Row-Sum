#! /bin/bash

clear
make

echo "************************************************************************************************************************************************"
time mpirun -np 4 ./main -gran 1 -seed 42 #-rows 16 -cols 10000000 
