#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIDE 9 // size of the Sudoku grid
#define SUBSIDE 3 // size of the sub-grid

int grid[SIDE][SIDE]; // Sudoku grid
int hints[SIDE][SIDE]; // hints loaded from file

// Function to check if a number can be placed in a cell
int is_valid(int row, int col, int num) {
    // Check row and column
    for (int i = 0; i < SIDE; i++) {
        if (grid[row][i] == num || grid[i][col] == num) {
            return 0;
        }
    }
    // Check sub-grid
    int sub_row = row - row % SUBSIDE;
    int sub_col = col - col % SUBSIDE;
    for (int i = 0; i < SUBSIDE; i++) {
        for (int j = 0; j < SUBSIDE; j++) {
            if (grid[sub_row + i][sub_col + j] == num) {
                return 0;
            }
        }
    }
    return 1;
}

// Function to solve the Sudoku puzzle
// void solve_sudoku() {
//     #pragma omp parallel
//     {
//         int thread_num = omp_get_thread_num();
//         int num_threads = omp_get_num_threads();

//         #pragma omp single
//         {
//             printf("Solving Sudoku puzzle with %d threads...\n", num_threads);
//         }

//         #pragma omp task
//         for (int row = 0; row < SIDE; row++) {
//             for (int col = 0; col < SIDE; col++) {
//                 if (hints[row][col] == 0) { // if cell is empty
//                     for (int num = 1; num <= SIDE; num++) {
//                         if (is_valid(row, col, num)) {
//                             grid[row][col] = num;
//                             #pragma omp task
//                             solve_sudoku(); // recursive call
//                             grid[row][col] = 0; // backtrack
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }
void solve_sudoku() {
    int i, j, num;
    #pragma omp parallel for private(i, j, num) schedule(dynamic)
    for (i = 0; i < SIDE; i++) {
        for (j = 0; j < SIDE; j++) {
            if (hints[i][j] == 0) { // if cell is empty
                for (num = 1; num <= SIDE; num++) {
                    if (is_valid(i, j, num)) {
                        grid[i][j] = num;
                        #pragma omp task
                        solve_sudoku(); // recursive call
                        grid[i][j] = 0; // backtrack
                    }
                }
            }
        }
    }
}

// Function to load hints from file
void load_hints(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }
    for (int row = 0; row < SIDE; row++) {
        for (int col = 0; col < SIDE; col++) {
            fscanf(file, "%d", &hints[row][col]);
            printf("num = %d\n", hints[row][col]);
        }
    }
    fclose(file);
}

int main() {
    load_hints("example.txt"); // load hints from file
   
    // Initialize grid with hints
    for (int row = 0; row < SIDE; row++) {
        for (int col = 0; col < SIDE; col++) {
            grid[row][col] = hints[row][col];
        }
    }
for (int row = 0; row < SIDE; row++) {
        for (int col = 0; col < SIDE; col++) {
            printf("%2d ", grid[row][col]);
        }
        printf("\n");
    }
    solve_sudoku();

    // Print solved grid
    for (int row = 0; row < SIDE; row++) {
        for (int col = 0; col < SIDE; col++) {
            printf("%2d ", grid[row][col]);
        }
        printf("\n");
    }

    return 0;
}