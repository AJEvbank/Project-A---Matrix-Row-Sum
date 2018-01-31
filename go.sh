#! /bin/bash
make
clear
time mpirun -np 4 ./main -gran 250
