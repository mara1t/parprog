#!/bin/bash

#EXECUTABLE="./mainpar"

gcc -fopenmp ../src/mainopenmp.c -o ../exec/mainopenmp -lm
mpicc -o ../exec/mainmpi ../src/mainmpi0.c -lm 
gcc ../src/main.c -o ../exec/main -lm

../exec/main

for i in {1..8}; do
    echo "Запуск программы 0 с $i процессами..."
    #gcc -fopenmp mainpar.c -o mainpar -lm #-D OMP_NUM_THREADS=$i
    #export OMP_NUM_THREADS=$i
    ../exec/mainopenmp $i
done

for i in {1..8}; do
    echo "Запуск программы 0 с $i процессами..."
    #gcc -fopenmp mainpar.c -o mainpar -lm #-D OMP_NUM_THREADS=$i
    #export OMP_NUM_THREADS=$i
    mpirun -np $i ../exec/mainmpi
done

echo "Difference:"
diff ../results/mainmpi.txt ../results/main.txt
diff ../results/mainopenmp.txt ../results/main.txt