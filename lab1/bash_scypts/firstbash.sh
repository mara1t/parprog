#!/bin/bash

EXECUTABLE="../exec/firstpar"

mpicc -o ../exec/firstpar ../src/2vpar.c -lm 
gcc ../src/2v.c -o ../exec/first -lm
../exec/first

# Цикл от 1 до 8
for i in {1..8}; do
    echo "Запуск программы 1 с $i процессами..."
    mpirun -np $i $EXECUTABLE
done

echo "Differnece":
diff ../results/2v.txt ../results/2vpar.txt