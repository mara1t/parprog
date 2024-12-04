#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define ISIZE 500
#define JSIZE 500

int main(int argc, char **argv)
{

    omp_set_num_threads((int)atoi(argv[1]));
    printf("%d\n", (int)atoi(argv[1]));
    double * * a = (double**) malloc(ISIZE * sizeof(double*));
    for (int i = 0; i < ISIZE; i++) {
        a[i] = (double*)malloc(JSIZE * sizeof(double));
    }
    int i, j;
    FILE *ff;

    
    for (i = 0; i < ISIZE; i++) {
        for (j = 0; j < JSIZE; j++) {
            a[i][j] = 10 * i + j;
        }
    }

    
    double start_time = omp_get_wtime();
    #pragma omp parallel for collapse(2) // for private(j) // Параллелизация внешнего цикла
    for (i = 0; i < ISIZE; i++) {
        for (j = 0; j < JSIZE; j++) {
            a[i][j] = sin(2 * a[i][j]);
        }
    }
    double end_time = omp_get_wtime(); // Конец измерения времени

    ff = fopen("../results/mainopenmp.txt", "w");
    for (i = 0; i < ISIZE; i++) {
        for (j = 0; j < JSIZE; j++) {
            fprintf(ff, "%f ", a[i][j]);
        }
        fprintf(ff, "\n");
    }

    fclose(ff);
    
    printf("Time taken: %f seconds\n", end_time - start_time);

    return 0;
}