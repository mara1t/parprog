#include <omp.h>
#include <stdio.h>

long long sumNsquared(int x) {
    long int sum = 0;
    for (int i = 1; i <= x; i++) {
        sum += i * i;
    }
    return sum;
}

int main() {
    omp_set_num_threads(4);
    long long result = 1;
    double start;
    double finish;
    start = omp_get_wtime();
    #pragma omp parallel for
        for (int i = 1; i < 129; i++){
            result = sumNsquared(i);
            //printf("thread: %d, iteration: %d\n", omp_get_thread_num(), i);
        }    
    finish = omp_get_wtime();
    printf("no balancing time: %f\n", finish - start);

    start = omp_get_wtime();
    #pragma omp parallel for schedule(static, 1)
        for (int i = 1; i < 129; i++){
            result = sumNsquared(i);
            //printf("thread: %d, iteration: %d\n", omp_get_thread_num(), i);
        }
    finish = omp_get_wtime();
    printf("static balancing 1 chunk time: %f\n", finish - start);

    start = omp_get_wtime();
    #pragma omp parallel for schedule(static, 4)
        for (int i = 1; i < 129; i++){
            result = sumNsquared(i);
            //printf("thread: %d, iteration: %d\n", omp_get_thread_num(), i);
        }
    finish = omp_get_wtime();
    printf("static balancing 4 chunks time: %f\n", finish - start);

    start = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic, 1)
        for (int i = 1; i < 129; i++){
            result = sumNsquared(i);
            //printf("thread: %d, iteration: %d\n", omp_get_thread_num(), i);
        }
    finish = omp_get_wtime();
    printf("dynamic balancing 1 chunk time: %f\n", finish - start);

    start = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic, 4)
        for (int i = 1; i < 129; i++){
            result = sumNsquared(i);
            //printf("thread: %d, iteration: %d\n", omp_get_thread_num(), i);
        }
    finish = omp_get_wtime();
    printf("dynamic balancing 4 chunks time: %f\n", finish - start);
    
    start = omp_get_wtime();
    #pragma omp parallel for schedule(guided)
        for (int i = 1; i < 129; i++){
            result = sumNsquared(i);
            // printf("thread: %d, iteration: %d\n", omp_get_thread_num(), i);
        }
    finish = omp_get_wtime();
    printf("guided balancing no chunk time: %f\n", finish - start);

    start = omp_get_wtime();
    #pragma omp parallel for schedule(guided, 4)
        for (int i = 1; i < 129; i++){
            result = sumNsquared(i);
            // printf("thread: %d, iteration: %d\n", omp_get_thread_num(), i);
        }
    finish = omp_get_wtime();
    printf("guided balancing 4 chunks time: %f\n", finish - start);

    return 0;
}