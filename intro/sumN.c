#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s N\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);

    double sum = 0;

    #pragma omp parallel
    {
        double thread_sum = 0;
        int thread_num = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        int start = (N / num_threads) * thread_num;
        int end = (thread_num == num_threads - 1) ? N : (N / num_threads) * (thread_num + 1);

        for (int i = start; i < end; i++) {
            thread_sum += ((double)1 / (i + 1));
            
        }
        
        #pragma omp critical
        {
            sum += thread_sum;
        }
    }

    printf("Sum: %lg\n", sum);

    return 0;
}