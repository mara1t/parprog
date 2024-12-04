#!/bin/bash
rm solve.txt
gcc -fopenmp -o lab lab.cpp -lm
./lab
