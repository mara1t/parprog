#!/bin/bash

EXECUTABLE="./secondpar"

gcc -fopenmp ../src/2bpar.c -o ../exec/secondpar -lm
gcc ../src/2b.c -o ../exec/second -lm

../exec/second
# Цикл от 1 до 8
for i in {1..8}; do
    echo "Запуск программы 2 с $i процессами..."
    #export OMP_NUM_THREADS=$i
    ../exec/secondpar $i
    
done

echo "Differnece":
diff ../results/2b.txt ../results/2bpar.txt