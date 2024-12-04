#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

const int N_THREAD = 1;
const int NUM_OF_NEWTON_ITERATIONS = 5;
const int N = 500;
const int H = 1/ (N - 1);
void solve(double **A, double *X, int n);
void yacobi(double **A, double *Y, int n);
void getBFromMatrixF(double *Y, int n, double *B);
void NewtonLin(double *Y, int n);

int main()
{
    double *Y = (double *)malloc(N * sizeof(double));
    Y[0] = 0;
    Y[N - 1] = 1;

    for (int i = 0; i < N; i++) {
        Y[i] = Y[0] + i * (Y[N - 1] - Y[0]) / (N - 1);
    }

    double start_time = omp_get_wtime();
    NewtonLin(Y, N);
    double end_time = omp_get_wtime();
    double time_taken = end_time - start_time;
    printf("Time taken: %f seconds\n", time_taken);
    printf("%f", Y[600]);
    FILE *ff = fopen("solve.txt", "w");
    for (int i = 0; i < N; i++) {
        fprintf(ff, "%f ", Y[i]);
    }
    fclose(ff);
    free(Y);

    return 0;
}

void solve(double **A, double *X, int n)
{
    omp_set_num_threads(N_THREAD);
    int i, j, k;
    for (k = 0; k < n - 1; k++) {
        #pragma omp parallel for private(i, j) shared(A, k, n)
        for (i = k + 1; i < n; i++) {
            double factor = A[i][k] / A[k][k];
            for (j = k; j < n + 1; j++) {
                A[i][j] -= factor * A[k][j];
            }
        }
    }
    for (i = n - 1; i >= 0; i--) {
        X[i] = A[i][n] / A[i][i];
        #pragma omp parallel for private(j) shared(A, X, i, n)
        for (j = 0; j < i; j++) {
            A[j][n] -= A[j][i] * X[i];
        }
    }
}

void yacobi(double **A, double *Y, int n)
{
    double h = 1.0/(n - 1);
    for (int i = 0; i < n - 2; i++) {
        for (int j = 0; j < n - 2; j++) {
            A[i][j] = 0;
        }
    }

    for (int i = 0; i < n - 2; i++) {
        A[i][i] = -2 - (10.0 / 12) * h * h * exp(Y[i + 1]);
    }

    for (int i = 0; i < n - 3; i++) {
        A[i][i + 1] = 1 - (h * h / 12) * exp(Y[i + 2]);
    }

    for (int i = 1; i < n - 2; i++) {
        A[i][i - 1] = 1 - (h * h / 12) * exp(Y[i]);
    }
}

void getBFromMatrixF(double *Y, int n, double *B)
{
    double h = 1.0 / (n - 1);
    for (int i = 0; i < n - 2; i++) {
        B[i] = -(Y[i + 2] - 2 * Y[i + 1] + Y[i] - h * h / 12 * (Y[i + 2] + 10 * Y[i + 1] + Y[i]));
    }
}

void NewtonLin(double *Y, int n)
{
    double**A = (double**) malloc((n - 2) * sizeof(double *));
    for (int i = 0; i < n - 2; i++) {
        A[i] = (double *)malloc((n - 1) * sizeof(double));
    }

    for (int i = 0; i < NUM_OF_NEWTON_ITERATIONS; i++) {
        yacobi(A, Y, n);
        double B[n - 2];
        getBFromMatrixF(Y, n, B);
        for (int j = 0; j < n - 2; j++) {
            A[j][n - 2] = B[j];
        }
        solve(A, B, n - 2);
        for (int j = 0; j < n - 2; j++) {
            Y[j + 1] += B[j];
        }
    }

    for (int i = 0; i < n - 2; i++) {
        free(A[i]);
    }
    free(A);
}
