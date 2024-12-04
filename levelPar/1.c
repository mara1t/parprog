#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel num_threads(4)
    {
        int thread_num = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        printf("Level 1: Thread %d of %d\n", thread_num, num_threads);

        #pragma omp parallel num_threads(2)
        {
            int thread_num2 = omp_get_thread_num();
            int num_threads2 = omp_get_num_threads();

            printf("Level 2: Thread %d of %d (parent thread %d of %d)\n",
                   thread_num2, num_threads2, thread_num, num_threads);

            #pragma omp parallel num_threads(2)
            {
                int thread_num3 = omp_get_thread_num();
                int num_threads3 = omp_get_num_threads();

                printf("Level 3: Thread %d of %d (parent thread %d of %d, grandparent thread %d of %d)\n",
                       thread_num3, num_threads3, thread_num2, num_threads2, thread_num, num_threads);
            }
        }
    }

    return 0;
}
