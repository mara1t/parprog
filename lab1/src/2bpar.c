#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define ISIZE 2000
#define JSIZE 2000

int main(int argc, char **argv)
{
    omp_set_num_threads((int)atoi(argv[1]));

    double * * a = (double**) malloc(ISIZE * sizeof(double*));
    for (int i = 0; i < ISIZE; i++) {
        a[i] = (double*)malloc(JSIZE * sizeof(double));
    }

    //double a[ISIZE][JSIZE];
    int i, j;
    FILE *ff;

    
    #pragma omp parallel for collapse(2) // for private(j) 
    for (i = 0; i < ISIZE; i++) {
        for (j = 0; j < JSIZE; j++) {
            a[i][j] = 10 * i + j;
        }
    }

    double start_time = omp_get_wtime();
    for (int i = 0; i < ISIZE - 3; i++)
    {
        #pragma omp parallel for
        for (int j = 2; j < JSIZE; j++)
        {
            a[i][j] = sin(0.1 * a[i + 3][j - 2]);
        }
    }


    double end_time = omp_get_wtime();
    
    ff = fopen("../results/2bpar.txt", "w");
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