/*Prepared by:
Shaimaa Dar Taha
Abeer Hussien
Section: 1
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h> //Include this for time function 
#include <string.h> //Include this for memset function 
#include <unistd.h>  // Include this for the sleep function
// Constants for 9x9 Sudoku
#define SIZE_9 9  // Size of the 9x9 Sudoku grid
#define SUBGRID_SIZE_9 3  // Size of the subgrid in 9x9 Sudoku
#define NUM_THREADS_11 11  // Number of threads for 11-thread validation (1 row + 1 column + 9 subgrid threads)
#define NUM_THREADS_27_9 27  // Number of threads for 27-thread validation (9 rows + 9 columns + 9 subgrids)

// Constants for 25x25 Sudoku
#define SIZE_25 25  // Size of the 25x25 Sudoku grid
#define SUBGRID_SIZE_25 5  // Size of the subgrid in 25x25 Sudoku
#define NUM_THREADS_27_25 27  // Number of threads for 27-thread validation (1 row + 1 column + 25 subgrid threads)
#define NUM_THREADS_75 75  // Number of threads for 75-thread validation (25 rows + 25 columns + 25 subgrids)

// Global Sudoku grids
int sudoku_9[SIZE_9][SIZE_9];  // 9x9 Sudoku grid
int sudoku_25[SIZE_25][SIZE_25];  // 25x25 Sudoku grid

// Results arrays to store validation results
int results_9[NUM_THREADS_27_9] = {0};  // Results for 9x9 validation
int results_25[NUM_THREADS_75] = {0};  // Results for 25x25 validation

// Parameter structures for thread functions
typedef struct {
    int row;  // Row index for validation
    int col;  // Column index for validation
    int index;  // Index in results array
} parameters;

typedef struct {
    int startRow;  // Starting row for subgrid validation
    int startCol;  // Starting column for subgrid validation
    int index;  // Index in results array
} subgrid_params;

/* ========== 9x9 Sudoku Functions ========== */

// Function to read a 9x9 Sudoku grid from a file
void read_sudoku_from_file_9(const char* filename);

// Function to print the 9x9 Sudoku grid
void print_sudoku_9();

// Function to validate a specific row in the 9x9 Sudoku grid
bool isValidRow_9(int row);

// Function to validate a specific column in the 9x9 Sudoku grid
bool isValidColumn_9(int col);

// Function to validate a specific subgrid in the 9x9 Sudoku grid
bool isValidSubgrid_9(int startRow, int startCol);

// Function to validate the entire 9x9 Sudoku grid using a naive approach
bool isValidSudoku_9();

// Thread function to check all rows in the 9x9 Sudoku grid
void* check_all_rows_9(void* arg);

// Thread function to check all columns in the 9x9 Sudoku grid
void* check_all_columns_9(void* arg);

// Thread function to check a specific subgrid in the 9x9 Sudoku grid
void* check_subgrid_9(void* param);

// Function to validate the 9x9 Sudoku grid using 11 threads
void validate_9x9_11_threads();

// Thread function to check a single row in the 9x9 Sudoku grid
void* check_single_row_9(void* arg);

// Thread function to check a single column in the 9x9 Sudoku grid
void* check_single_column_9(void* arg);

// Thread function to check a single subgrid in the 9x9 Sudoku grid
void* check_single_subgrid_9(void* arg);

// Function to validate the 9x9 Sudoku grid using 27 threads
void validate_9x9_27_threads();

/* ========== 25x25 Sudoku Functions ========== */

// Function to read a 25x25 Sudoku grid from a file
void read_sudoku_from_file_25(const char* filename);

// Function to print the 25x25 Sudoku grid
void print_sudoku_25();

// Function to validate a specific row in the 25x25 Sudoku grid
bool isValidRow_25(int row);

// Function to validate a specific column in the 25x25 Sudoku grid
bool isValidColumn_25(int col);

// Function to validate a specific subgrid in the 25x25 Sudoku grid
bool isValidSubgrid_25(int startRow, int startCol);

// Function to validate the entire 25x25 Sudoku grid using a naive approach
bool isValidSudoku_25();

// Function to validate the 25x25 Sudoku grid using 27 threads
void validate_25x25_27_threads();

// Function to validate the 25x25 Sudoku grid using 75 threads
void validate_25x25_75_threads();

/* ========== Main Function ========== */

int main() {
    int option;
    while(1) {
        printf("\nChoose the size of the SUDOKU game you want:\n");
        printf("1. 9x9 Sudoku\n");
        printf("2. 25x25 Sudoku\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &option);

        if (option == 3) {
            printf("Thank you for using the program.\n");
            break;
        }

        if (option == 1) {
            // 9x9 Sudoku
            read_sudoku_from_file_9("Sudoku_9.txt");
            print_sudoku_9();

            int choice_9;
            while (1) {
                printf("\nChoose validation method for 9x9 Sudoku:\n");
                printf("1. Naive (no threads)\n");
                printf("2. 11 Threads (1 row, 1 column, 9 subgrids)\n");
                printf("3. 27 Threads (9 rows, 9 columns, 9 subgrids)\n");
                printf("4. Back to main menu\n");
                printf("Enter your choice: ");
                scanf("%d", &choice_9);

                if (choice_9 == 4) break;

                clock_t start = clock();

                switch(choice_9) {
                    case 1:

                        printf(isValidSudoku_9() ? "Sudoku is valid (Naive).\n" : "Sudoku is NOT valid (Naive).\n");

                        break;
                    case 2:

                        validate_9x9_11_threads();

                        break;
                    case 3:

                        validate_9x9_27_threads();

                        break;
                    default:
                        printf("Invalid choice, try again.\n");

                        continue;
                }
                 sleep(1);
                clock_t end = clock();
                printf("Execution time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
                printf("=====================================\n");
            }
        }
        else if (option == 2) {
            // 25x25 Sudoku
            read_sudoku_from_file_25("Sudoku25.txt");
            print_sudoku_25();

            int choice_25;
            while (1) {
                printf("\nChoose validation method for 25x25 Sudoku:\n");
                printf("1. Naive (no threads)\n");
                printf("2. 27 Threads (1 row, 1 column, 25 subgrids)\n");
                printf("3. 75 Threads (25 rows, 25 columns, 25 subgrids)\n");
                printf("4. Back to main menu\n");
                printf("Enter your choice: ");
                scanf("%d", &choice_25);

                if (choice_25 == 4) break;

                clock_t start = clock();

                switch(choice_25) {
                    case 1:

                        printf(isValidSudoku_25() ? "Sudoku is valid (Naive).\n" : "Sudoku is NOT valid (Naive).\n");
                        break;
                    case 2:

                        validate_25x25_27_threads();
                        break;
                    case 3:

                        validate_25x25_75_threads();
                        break;
                    default:
                        printf("Invalid choice, try again.\n");
                        continue;
                }
                sleep(1);

                clock_t end = clock();
                printf("Execution time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
                 printf("=====================================\n");
            }
        }
        else {
            printf("Invalid choice, try again.\n");
        }
    }
    return 0;
}

// File & Utility Functions
// Function to read Sudoku grid from a file
void read_sudoku_from_file_9(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {  // If file cannot be opened
        printf("Failed to open file.\n");
        exit(1);  // Exit the program if file is not found
    }

    // Read the grid from file line by line
    for (int i = 0; i < SIZE_9; i++) {
        fscanf(file, " {");
        for (int j = 0; j < SIZE_9; j++) {
            fscanf(file, " %d", &sudoku_9[i][j]);  // Read each number in the row
            if (j < SIZE_9 - 1) fscanf(file, " ,");  // Skip commas between numbers
        }
        fscanf(file, " } ,");  // Read the closing brace and comma at the end of each row
    }

    fclose(file);  // Close the file after reading
}

void print_sudoku_9() {
    printf("9x9 Sudoku Grid:\n");
    for (int i = 0; i < SIZE_9; i++) {
        for (int j = 0; j < SIZE_9; j++) {
            printf("%2d ", sudoku_9[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Validation functions
bool isValidRow_9(int row) {
    bool seen[SIZE_9 + 1] = {false};
    for (int j = 0; j < SIZE_9; j++) {
        int val = sudoku_9[row][j];
        if (val < 1 || val > SIZE_9 || seen[val]) return false;
        seen[val] = true;
    }
    return true;
}

bool isValidColumn_9(int col) {
    bool seen[SIZE_9 + 1] = {false};
    for (int i = 0; i < SIZE_9; i++) {
        int val = sudoku_9[i][col];
        if (val < 1 || val > SIZE_9 || seen[val]) return false;
        seen[val] = true;
    }
    return true;
}

bool isValidSubgrid_9(int startRow, int startCol) {
    bool seen[SIZE_9 + 1] = {false};
    for (int i = startRow; i < startRow + SUBGRID_SIZE_9; i++) {
        for (int j = startCol; j < startCol + SUBGRID_SIZE_9; j++) {
            int val = sudoku_9[i][j];
            if (val < 1 || val > SIZE_9 || seen[val]) return false;
            seen[val] = true;
        }
    }
    return true;
}

// Naive approach (single-threaded)
bool isValidSudoku_9() {
    // Check all rows
    for (int i = 0; i < SIZE_9; i++) {
        if (!isValidRow_9(i)) return false;
    }

    // Check all columns
    for (int j = 0; j < SIZE_9; j++) {
        if (!isValidColumn_9(j)) return false;
    }

    // Check all subgrids
    for (int i = 0; i < SIZE_9; i += SUBGRID_SIZE_9) {
        for (int j = 0; j < SIZE_9; j += SUBGRID_SIZE_9) {
            if (!isValidSubgrid_9(i, j)) return false;
        }
    }

    return true;
}

// 11-thread approach (1 row thread + 1 column thread + 9 subgrid threads)
void* check_all_rows_9(void* arg) {
    for (int i = 0; i < SIZE_9; i++) {
        if (!isValidRow_9(i)) {
            results_9[0] = 0;
            pthread_exit(NULL);
        }
    }
    results_9[0] = 1;
    pthread_exit(NULL);
}

void* check_all_columns_9(void* arg) {
    for (int j = 0; j < SIZE_9; j++) {
        if (!isValidColumn_9(j)) {
            results_9[1] = 0;
            pthread_exit(NULL);
        }
    }
    results_9[1] = 1;
    pthread_exit(NULL);
}

void* check_subgrid_9(void* param) {
    parameters* p = (parameters*)param;
    results_9[p->index] = isValidSubgrid_9(p->row, p->col) ? 1 : 0;
    free(p);
    pthread_exit(NULL);
}

void validate_9x9_11_threads() {
    memset(results_9, 0, sizeof(results_9));//to clear the array 
    pthread_t threads[NUM_THREADS_11];

    // Thread 0: Check all rows
    pthread_create(&threads[0], NULL, check_all_rows_9, NULL);

    // Thread 1: Check all columns
    pthread_create(&threads[1], NULL, check_all_columns_9, NULL);

    // Threads 2-10: Check each subgrid (9 threads)
    int thread_idx = 2;
    for (int i = 0; i < SIZE_9; i += SUBGRID_SIZE_9) {
        for (int j = 0; j < SIZE_9; j += SUBGRID_SIZE_9) {
            parameters* params = malloc(sizeof(parameters));
            params->row = i;
            params->col = j;
            params->index = thread_idx;
            pthread_create(&threads[thread_idx], NULL, check_subgrid_9, params);
            thread_idx++;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS_11; i++) {
        pthread_join(threads[i], NULL);
    }

    // Check results
    bool valid = true;
    for (int i = 0; i < NUM_THREADS_11; i++) {
        if (results_9[i] != 1) {
            valid = false;
            break;
        }
    }
    printf(valid ? "Sudoku is valid (11-thread).\n" : "Sudoku is NOT valid (11-thread).\n");
}

// 27-thread approach (9 row threads + 9 column threads + 9 subgrid threads)
void* check_single_row_9(void* arg) {
    parameters* p = (parameters*)arg;
    results_9[p->index] = isValidRow_9(p->row) ? 1 : 0;
    free(p);
    pthread_exit(NULL);
}

void* check_single_column_9(void* arg) {
    parameters* p = (parameters*)arg;
    results_9[p->index] = isValidColumn_9(p->col) ? 1 : 0;
    free(p);
    pthread_exit(NULL);
}

void* check_single_subgrid_9(void* arg) {
    parameters* p = (parameters*)arg;
    results_9[p->index] = isValidSubgrid_9(p->row, p->col) ? 1 : 0;
    free(p);
    pthread_exit(NULL);
}

void validate_9x9_27_threads() {
    memset(results_9, 0, sizeof(results_9));//to clear the array 
    pthread_t threads[NUM_THREADS_27_9];
    int thread_idx = 0;

    // Create 9 row threads (1 per row)
    for (int i = 0; i < SIZE_9; i++) {
        parameters* params = malloc(sizeof(parameters));
        params->row = i;
        params->index = thread_idx;
        pthread_create(&threads[thread_idx], NULL, check_single_row_9, params);
        thread_idx++;
    }

    // Create 9 column threads (1 per column)
    for (int j = 0; j < SIZE_9; j++) {
        parameters* params = malloc(sizeof(parameters));
        params->col = j;
        params->index = thread_idx;
        pthread_create(&threads[thread_idx], NULL, check_single_column_9, params);
        thread_idx++;
    }

    // Create 9 subgrid threads (1 per subgrid)
    for (int i = 0; i < SIZE_9; i += SUBGRID_SIZE_9) {
        for (int j = 0; j < SIZE_9; j += SUBGRID_SIZE_9) {
            parameters* params = malloc(sizeof(parameters));
            params->row = i;
            params->col = j;
            params->index = thread_idx;
            pthread_create(&threads[thread_idx], NULL, check_single_subgrid_9, params);
            thread_idx++;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS_27_9; i++) {
        pthread_join(threads[i], NULL);
    }

    // Check results
    bool valid = true;
    for (int i = 0; i < NUM_THREADS_27_9; i++) {
        if (results_9[i] != 1) {
            valid = false;
            break;
        }
    }
    printf(valid ? "Sudoku is valid (27-thread).\n" : "Sudoku is NOT valid (27-thread).\n");
}

/* ========== 25x25 Sudoku Functions ========== */

// File & Utility Functions
void read_sudoku_from_file_25(const char* filename) {
     FILE* file = fopen(filename, "r");
    if (!file) {  // If file cannot be opened
        printf("Failed to open file.\n");
        exit(1);  // Exit the program if file is not found
    }

    // Read the grid from file line by line
    for (int i = 0; i < SIZE_25; i++) {
        fscanf(file, " {");
        for (int j = 0; j < SIZE_25; j++) {
            fscanf(file, " %d", &sudoku_25[i][j]);  // Read each number in the row
            if (j < SIZE_25 - 1) fscanf(file, " ,");  // Skip commas between numbers
        }
        fscanf(file, " } ,");  // Read the closing brace and comma at the end of each row
    }

    fclose(file);  // Close the file after reading
}

void print_sudoku_25() {
    printf("25x25 Sudoku Grid:\n");
    for (int i = 0; i < SIZE_25; i++) {
        for (int j = 0; j < SIZE_25; j++) {
            printf("%2d ", sudoku_25[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Validation functions
bool isValidRow_25(int row) {
    bool seen[SIZE_25 + 1] = {false};
    for (int j = 0; j < SIZE_25; j++) {
        int val = sudoku_25[row][j];
        if (val < 1 || val > SIZE_25 || seen[val]) return false;
        seen[val] = true;
    }
    return true;
}

bool isValidColumn_25(int col) {
    bool seen[SIZE_25 + 1] = {false};
    for (int i = 0; i < SIZE_25; i++) {
        int val = sudoku_25[i][col];
        if (val < 1 || val > SIZE_25 || seen[val]) return false;
        seen[val] = true;
    }
    return true;
}

bool isValidSubgrid_25(int startRow, int startCol) {
    bool seen[SIZE_25 + 1] = {false};
    for (int i = startRow; i < startRow + SUBGRID_SIZE_25; i++) {
        for (int j = startCol; j < startCol + SUBGRID_SIZE_25; j++) {
            int val = sudoku_25[i][j];
            if (val < 1 || val > SIZE_25 || seen[val]) return false;
            seen[val] = true;
        }
    }
    return true;
}

// Naive approach (single-threaded)
bool isValidSudoku_25() {
    // Check all rows
    for (int i = 0; i < SIZE_25; i++) {
        if (!isValidRow_25(i)) return false;
    }

    // Check all columns
    for (int j = 0; j < SIZE_25; j++) {
        if (!isValidColumn_25(j)) return false;
    }

    // Check all subgrids
    for (int i = 0; i < SIZE_25; i += SUBGRID_SIZE_25) {
        for (int j = 0; j < SIZE_25; j += SUBGRID_SIZE_25) {
            if (!isValidSubgrid_25(i, j)) return false;
        }
    }

    return true;
}

// 27-thread approach (1 row thread + 1 column thread + 25 subgrid threads)
void* check_all_rows_25(void* arg) {
    for (int i = 0; i < SIZE_25; i++) {
        if (!isValidRow_25(i)) {
            results_25[0] = 0;
            pthread_exit(NULL);
        }
    }
    results_25[0] = 1;
    pthread_exit(NULL);
}

void* check_all_columns_25(void* arg) {
    for (int j = 0; j < SIZE_25; j++) {
        if (!isValidColumn_25(j)) {
            results_25[1] = 0;
            pthread_exit(NULL);
        }
    }
    results_25[1] = 1;
    pthread_exit(NULL);
}

void* check_single_subgrid_25(void* arg) {
    subgrid_params* p = (subgrid_params*)arg;
    results_25[p->index] = isValidSubgrid_25(p->startRow, p->startCol) ? 1 : 0;
    free(p);
    pthread_exit(NULL);
}

void validate_25x25_27_threads() {
    pthread_t threads[NUM_THREADS_27_25];

    // Thread 0: Check all rows
    pthread_create(&threads[0], NULL, check_all_rows_25, NULL);

    // Thread 1: Check all columns
    pthread_create(&threads[1], NULL, check_all_columns_25, NULL);

    // Threads 2-26: Check each subgrid (25 threads)
    int thread_idx = 2;
    for (int i = 0; i < SIZE_25; i += SUBGRID_SIZE_25) {
        for (int j = 0; j < SIZE_25; j += SUBGRID_SIZE_25) {
            subgrid_params* params = malloc(sizeof(subgrid_params));
            params->startRow = i;
            params->startCol = j;
            params->index = thread_idx;
            pthread_create(&threads[thread_idx], NULL, check_single_subgrid_25, params);
            thread_idx++;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS_27_25; i++) {
        pthread_join(threads[i], NULL);
    }

    // Check results
    bool valid = true;
    for (int i = 0; i < NUM_THREADS_27_25; i++) {
        if (results_25[i] != 1) {
            valid = false;
            break;
        }
    }
    printf(valid ? "Sudoku is valid (27-thread).\n" : "Sudoku is NOT valid (27-thread).\n");
}

// 75-thread approach (25 row threads + 25 column threads + 25 subgrid threads)
void* check_single_row_25(void* arg) {
    parameters* p = (parameters*)arg;
    results_25[p->index] = isValidRow_25(p->row) ? 1 : 0;
    free(p);
    pthread_exit(NULL);
}

void* check_single_column_25(void* arg) {
    parameters* p = (parameters*)arg;
    results_25[p->index] = isValidColumn_25(p->col) ? 1 : 0;
    free(p);
    pthread_exit(NULL);
}

void* check_single_subgrid_25_full(void* arg) {
    subgrid_params* p = (subgrid_params*)arg;
    results_25[p->index] = isValidSubgrid_25(p->startRow, p->startCol) ? 1 : 0;
    free(p);
    pthread_exit(NULL);
}

void validate_25x25_75_threads() {
    pthread_t threads[NUM_THREADS_75];
    int thread_idx = 0;

    // Create 25 row threads (1 per row)
    for (int i = 0; i < SIZE_25; i++) {
        parameters* params = malloc(sizeof(parameters));
        params->row = i;
        params->index = thread_idx;
        pthread_create(&threads[thread_idx], NULL, check_single_row_25, params);
        thread_idx++;
    }

    // Create 25 column threads (1 per column)
    for (int j = 0; j < SIZE_25; j++) {
        parameters* params = malloc(sizeof(parameters));
        params->col = j;
        params->index = thread_idx;
        pthread_create(&threads[thread_idx], NULL, check_single_column_25, params);
        thread_idx++;
    }

    // Create 25 subgrid threads (1 per subgrid)
    for (int i = 0; i < SIZE_25; i += SUBGRID_SIZE_25) {
        for (int j = 0; j < SIZE_25; j += SUBGRID_SIZE_25) {
            subgrid_params* params = malloc(sizeof(subgrid_params));
            params->startRow = i;
            params->startCol = j;
            params->index = thread_idx;
            pthread_create(&threads[thread_idx], NULL, check_single_subgrid_25_full, params);
            thread_idx++;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS_75; i++) {
        pthread_join(threads[i], NULL);
    }

    // Check results
    bool valid = true;
    for (int i = 0; i < NUM_THREADS_75; i++) {
        if (results_25[i] != 1) {
            valid = false;
            break;
        }
    }
    printf(valid ? "Sudoku is valid (75-thread).\n" : "Sudoku is NOT valid (75-thread).\n");
}

