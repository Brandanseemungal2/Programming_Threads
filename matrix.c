#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_SIZE 20
int MAX;
int matA[MAX_SIZE][MAX_SIZE]; 
int matB[MAX_SIZE][MAX_SIZE]; 
int matSumResult[MAX_SIZE][MAX_SIZE];
int matDiffResult[MAX_SIZE][MAX_SIZE]; 
int matProductResult[MAX_SIZE][MAX_SIZE]; 

typedef struct {
    int startRow;
    int endRow;
} ThreadArgs;

void fillMatrix(int matrix[MAX_SIZE][MAX_SIZE]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            matrix[i][j] = rand()%10+1;
        }
    }
}

void printMatrix(int matrix[MAX_SIZE][MAX_SIZE]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the sum of the
// values at the coordinates of matA and matB.
void* computeSum(void* args) { // pass in the number of the ith thread
    ThreadArgs* threadArgs = (ThreadArgs*)args;
    for (int i = threadArgs->startRow; i < threadArgs->endRow; i++) {
        for (int j = 0; j < MAX; j++) {
            matSumResult[i][j] = matA[i][j] + matB[i][j];
        }
    }
    free(threadArgs);
    pthread_exit(NULL);
}

void* computeDiff(void* args) {
    ThreadArgs* threadArgs = (ThreadArgs*)args;
    for (int i threadArgs->startRow; i < threadArgs->endRow; i++) {
        for (int j = 0; j < MAX; j++) {
            matDiffResult[i][j] = matA[i][j] - matB[i][j];
        }
    }
    free(threadArgs);
    pthread_exit(NULL);
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the inner product
// of matA and matB.
void* computeProduct(void* args) { // pass in the number of the ith thread
    ThreadArgs* threadArgs = (ThreadArgs*)args;
    for (int i = threadArgs->startRow; i < threadArgs->endRow; i++) {
        for (int j = 0; j < MAX; j++) {
            matProductResult[i][j] = 0;
            for (int k = 0; k < MAX, k++) {
                matProductResult[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
    free(threadArgs);
    pthread_exit(NULL);
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main(int argc, char* argv[]) {
    srand(time(0));  // Do Not Remove. Just ignore and continue below.

    if (argc != 2) {
        printf("Usage: %s <matrix size>\n", argv[0]);
        return 1;
    }
    
    MAX = atoi(argv[1]);
    if (MAX > MAX_SIZE || MAX <= 0) {
        printf("Matrix size must be between 1 and %d.\n", MAX_SIZE);
        return 1;
    }

    fillMatrix(matA);
    fillMatrix(matB);
    // 0. Get the matrix size from the command line and assign it to MAX
    
    // 1. Fill the matrices (matA and matB) with random values.
    
    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);
    
    // 3. Create pthread_t objects for our threads.
    pthread_t threads[10];
    int rowsPerThread = MAX / 10;

    for (int i = 0; i < 10; i++) {
        ThreadArgs* args = (ThreadArgs*)malloc(sizeof(ThreadArgs));
        args->startRow = i * rowsPerThread;
        args->endRow = (i == 9) ? MAX : (i + 1) * rowsPerThread;
        pthread_create(&threads[i], NULL, computeSum, args);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0, i < 10, i++) {
        ThreadArgs* args = (ThreadArgs*)malloc(sizeof(ThreadArgs));
        args->startRow = i * rowsPerThread;
        args->endRow = (i == 9) ? MAX : (i + 1) * rowsPerThread;
        pthread_create(&threads[i], NULL, computeDiff, args);
    }
    for (int i = 0; i < 10, i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < 10; i++) {
        ThreadArgs* args = (ThreadArgs*)malloc(sizeof(ThreadArgs));
        args->startRow = i * rowsPerThread;
        args->endRow = (i == 9) ? MAX : (i + 1) * rowsPerThread;
        pthread_create(&threads[i], NULL, computeProduct, args);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }
    
    //Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    return 0;
  
}
