#include <stdio.h>
#include <math.h>
#include <time.h>

#define ISIZE 1000
#define JSIZE 1000

int main(int argc, char **argv)
{
    double a[ISIZE][JSIZE];
    int i, j;
    FILE *ff;
   
    for (i=0; i<ISIZE; i++){
        for (j=0; j<JSIZE; j++){
            a[i][j] = 10*i +j;
        }
    }
    clock_t start = clock();
    for (i=3; i<ISIZE; i++){
        for (j = 0; j < JSIZE-2; j++){
            a[i][j] = sin(3*a[i -3][j+2]);
        }
    }
    clock_t end = clock();
    printf("Time taken: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    ff = fopen("../results/2v.txt","w");
    for(i=0; i < ISIZE; i++){
        for (j=0; j < JSIZE; j++){
            fprintf(ff,"%f ",a[i][j]);
        }
        fprintf(ff,"\n");
    }
    fclose(ff);
}