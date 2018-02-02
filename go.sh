#! /bin/bash
#make
#clear
time mpirun -np 4 ./main -gran 2500
time mpirun -np 4 ./main -gran 2500

time mpirun -np 4 ./main -gran 1250
time mpirun -np 4 ./main -gran 1250

time mpirun -np 4 ./main -gran 625
time mpirun -np 4 ./main -gran 625

time mpirun -np 4 ./main -gran 250
time mpirun -np 4 ./main -gran 250

time mpirun -np 4 ./main -gran 125
time mpirun -np 4 ./main -gran 125

time mpirun -np 4 ./main -gran 25
time mpirun -np 4 ./main -gran 25

time mpirun -np 4 ./main -gran 1
time mpirun -np 4 ./main -gran 1
