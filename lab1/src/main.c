#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define ISIZE 500
#define JSIZE 500
int main(int argc, char **argv)
{
    time_t start, end;
    double * * a = (double**) malloc(ISIZE * sizeof(double*));
    for (int i = 0; i < ISIZE; i++) {
        a[i] = (double*)malloc(JSIZE * sizeof(double));
    }
    int i, j;
    FILE *ff;
    //подготовительная часть – заполнение некими данными
    for (i=0; i<ISIZE; i++){
    for (j=0; j<JSIZE; j++){
    a[i][j] = 10*i +j;
    }
    }
    start = time(NULL);
    // требуется обеспечить измерение времени работы данного цикла
    for (i=0; i<ISIZE; i++){
        for (j = 0; j < JSIZE; j++){
            a[i][j] = sin(2*a[i][j]);
        }
    }
    end = time(NULL);
        
        // Вывод времени выполнения
    printf("Time taken: %ld seconds\n", (long)difftime(end, start));
    ff = fopen("../results/main.txt","w");
    for(i=0; i < ISIZE; i++){
        for (j=0; j < JSIZE; j++){
            fprintf(ff,"%f ",a[i][j]);
        }
        fprintf(ff,"\n");
    }
    fclose(ff);
}