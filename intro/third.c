#include <stdio.h>
#include <omp.h>

int main() {
    int shared_var = 0; // shared variable of type int

    #pragma omp parallel shared(shared_var)
    {
        int thread_num = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        #pragma omp barrier 

        for (int i = 0; i < num_threads; i++) {
            #pragma omp barrier

            if (i == thread_num) {
                shared_var++;
                printf("Thread %d: shared_var = %d\n", thread_num, shared_var);
            }
        }
    }

    return 0;
}