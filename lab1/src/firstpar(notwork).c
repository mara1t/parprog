#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define ISIZE 1000
#define JSIZE 1000

int main(int argc, char **argv)
{
    int rank, size;
    double a[ISIZE][JSIZE];
    int i, j;
    FILE *ff;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // int rows_per_process = (ISIZE - 3) / size; // Количество строк на процесс
    // int start_row = 3 + rank * rows_per_process; // Начальная строка для процесса
    // int end_row = (rank == size - 1) ? ISIZE : start_row + rows_per_process; // Конечная строка для процесса

    int rows_per_process = ISIZE / size;
    int start_row = rank * rows_per_process;
    int end_row = (rank == size - 1) ? ISIZE : start_row + rows_per_process;

    for (i = start_row; i < end_row; i++) {
        for (j = 0; j < JSIZE; j++) {
            a[i][j] = 10 * i + j;
        }
    }

    double start_time = MPI_Wtime();

    for (i = start_row + 3; i < end_row; i++) {
        for (j = 0; j < JSIZE - 2; j++) {
            a[i][j] = sin(3 * a[i - 3][j + 2]);
        }
    }
    double end_time = MPI_Wtime();

    if (rank != 0) {
        MPI_Send(&a[start_row], (end_row - start_row) * JSIZE * sizeof(double), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
    } else {
        for (int p = 1; p < size; p++) {
            int p_start_row = p * rows_per_process;
            int p_end_row = (p == size - 1) ? ISIZE : p_start_row + rows_per_process;
            MPI_Recv(&a[p_start_row], (p_end_row - p_start_row) * JSIZE * sizeof(double), MPI_BYTE, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    if (rank == 0) {
        ff = fopen("../results/first1.txt", "w");
        for (i = 0; i < ISIZE; i++) {
            for (j = 0; j < JSIZE; j++) {
                fprintf(ff, "%f ", a[i][j]);
            }
            fprintf(ff, "\n");
        }
        fclose(ff);
    }
    // if (rank == 0) {
    //     double *result = (double *)malloc(ISIZE * JSIZE * sizeof(double));
    //     MPI_Gather(a, rows_per_process * JSIZE, MPI_DOUBLE, result, rows_per_process * JSIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        
    //     // Обработка последнего процесса, если он не делит массив на равные части
    //     if (size > 1) {
    //         MPI_Gather(a + (size - 1) * rows_per_process, (ISIZE - (size - 1) * rows_per_process) * JSIZE, MPI_DOUBLE, result, (ISIZE - (size - 1) * rows_per_process) * JSIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    //     }

    //     // Запись результатов в файл
    //     ff = fopen("result10.txt", "w");
    //     if (ff == NULL) {
    //         fprintf(stderr, "Error opening file!\n");
    //         free(result);
    //         MPI_Finalize();
    //         return 1;
    //     }
        
    //     for (i = 0; i < ISIZE; i++) {
    //         for (j = 0; j < JSIZE; j++) {
    //             fprintf(ff, "%f ", result[i * JSIZE + j]);
    //         }
    //         fprintf(ff, "\n");
    //     }
        
    //     fclose(ff);
    //     free(result);
    // } else {
    //     // Отправка данных на главный процесс
    //     MPI_Gather(a, rows_per_process * JSIZE, MPI_DOUBLE, NULL, rows_per_process * JSIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    // }

    if (rank == 0) {
        printf("Time taken: %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}