#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define ISIZE 2000
#define JSIZE 2000

int main(int argc, char **argv)
{
    //time_t start, end;
    double * * a = (double**) malloc(ISIZE * sizeof(double*));
    for (int i = 0; i < ISIZE; i++) {
        a[i] = (double*)malloc(JSIZE * sizeof(double));
    }
    int i, j;
    FILE *ff;
    for (i=0; i<ISIZE; i++){
        for (j=0; j<JSIZE; j++){
            a[i][j] = 10*i +j;
        }
    }
    //start = time(NULL);
    clock_t start = clock();
    for (i=0; i<ISIZE-3; i++){
        for (j = 2; j < JSIZE; j++){
            a[i][j] = sin(0.1*a[i+3][j-2]);
        }
    }
    
    ff = fopen("../results/2b.txt","w");
    for(i=0; i < ISIZE; i++){
        for (j=0; j < JSIZE; j++){
            fprintf(ff,"%f ",a[i][j]);
        }
        fprintf(ff,"\n");   
    }
    fclose(ff);
    clock_t end = clock();
//end = time(NULL);
        
        // Вывод времени выполнения
    printf("Time taken: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}