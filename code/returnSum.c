/*
Pass a struct as input argument to a function run with pthread_create
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXTHREAD 4
#define N 1000

typedef struct {
    int* array;
    int start;
    int end;
} vector_t;

int array[N];
vector_t params[MAXTHREAD];

// Fill array elements
void init_array(int array[]) {
    for (int i = 0; i < N; i++)
        array[i] = i;
}

// Create params 
void init_params(vector_t params[]) {
    for (int i = 0; i < MAXTHREAD; i++) {
        int start = i * N / MAXTHREAD;
        int end = (i + 1) * N / MAXTHREAD;
        vector_t vector = {array, start, end};
        params[i] = vector;
    }
}

vector_t sub_vector(vector_t* parent, int start, int end) {
    vector_t result = {parent->array, start, end};
    return result;
}

void* getSum(void* args) {
    vector_t* vector = (vector_t*)args;
    int* local_sum = (int*)malloc(sizeof(int));
    *local_sum = 0;
    for (int i = vector->start; i < vector->end; i++)
        *local_sum += vector->array[i];

    printf("Start: %d, end: %d, local sum: %d\n", vector->start, vector->end,
           *local_sum);
    return (void*)local_sum;
}

int main() {
    init_array(array);
    init_params(params);
    pthread_t threads[MAXTHREAD];

    int* local_sum[MAXTHREAD];
    int global_sum = 0;

    // Start threads
    for (int i = 0; i < MAXTHREAD; i++) {
        pthread_create(&threads[i], NULL, getSum, &params[i]);
    }

    // Join threads 
    for (int i = 0; i < MAXTHREAD; i++) {
        pthread_join(threads[i], (void**)&local_sum[i]);
        global_sum += *local_sum[i];
        free(local_sum[i]);
    }

    printf("Sum of vector is: %d\n", global_sum);

    return 0;
}