#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define ISIZE 500
#define JSIZE 500

int main(int argc, char **argv)
{   

    int i, j;
    FILE *ff;
    double end_time, start_time;

    double ** temp; 
    double a[ISIZE][JSIZE];
    // double * * a = (double**) malloc(ISIZE * sizeof(double*));
    // if (a == NULL)
    //     {
    //         printf("Memory allocation failed for columns!\n");
    //         return 1;
    //     }
    // for (int i = 0; i < ISIZE; i++) {
    //     a[i] = (double*)malloc(JSIZE * sizeof(double));
    //     if (a[i] == NULL)
    //     {
    //         printf("Memory allocation failed for columns!\n");
    //         return 1;
    //     }
    // }
    // double * * temp = (double**) malloc(ISIZE * sizeof(double*));
    // for (int i = 0; i < ISIZE; i++) {
    //     temp[i] = (double*)malloc(JSIZE * sizeof(double));
    // }
     
    

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (i = 0; i < ISIZE; i++) {
        for (j = 0; j < JSIZE; j++) {
            a[i][j] = 10 * i + j;
        }
    }
    int rows_per_process = ISIZE / size;
    int start_row = rank * rows_per_process;
    int end_row =  (rank == size - 1) ? (ISIZE) : (start_row + rows_per_process);

    if (rank == 0) {
        start_time = MPI_Wtime();
    }

    for (i = start_row; i < end_row; i++) {
        for (j = 0; j < JSIZE; j++) {
            a[i][j] = sin(2 * a[i][j]);
        }
    }

    if (rank == 0) {

        for (i = 1; i < size; i++) {
            int rows = (i == size - 1)  && (ISIZE % size != 0)? (ISIZE - rows_per_process*i) : rows_per_process;
            MPI_Recv(&a[rows_per_process * i][0], JSIZE * rows , MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
    else {
        MPI_Send(&a[start_row][0], (end_row - start_row) * JSIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        end_time = MPI_Wtime();

        printf("Time taken: %f seconds\n", end_time - start_time);
    }

    if (rank == 0) {
        ff = fopen("../results/mainmpi.txt", "w");
        for (i = 0; i < ISIZE; i++) {
            for (j = 0; j < JSIZE; j++) {
                fprintf(ff, "%f ", a[i][j]);
            }
            fprintf(ff, "\n");
        }
        fclose(ff);
    }

    MPI_Finalize();
    return 0;
}