/*
Use pthreads to calculate sum of a large array. 
Local sum is written to a global variable called result.
Hoang Son Le a1691819 7/10/20203
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXTHREAD 4
#define N 1000

int array[N];
int result[MAXTHREAD];

void init_array(int array[]) {
    for (int i = 0; i < N; i++)
        array[i] = i;
}

void init_params(int params[]) {
    for (int i = 0; i < MAXTHREAD; i++)
        params[i] = i;
}

void* thread_sum(void* args);

int main() {
    pthread_t threads[MAXTHREAD];
    init_array(array);
    int params[MAXTHREAD];
    init_params(params);
    int global_result = 0;

    for (int i = 0; i < MAXTHREAD; i++) {
        pthread_create(&threads[i], NULL, thread_sum, (void*) (params + i));
    }

    for (int i = 0; i < MAXTHREAD; i++) {
        pthread_join(threads[i], NULL);
        global_result += result[i];
    }

    printf("Global result: %d\n", global_result);

    return 0;
}

void* thread_sum(void* arg) {
    int id = *(int*)arg;
    int start = id * N / MAXTHREAD;
    int end = (id + 1) * N / MAXTHREAD;
    int local_sum = 0;
    for (int i = start; i < end; i++)
        local_sum += array[i];
    printf("Thread id: %d, start: %d, end: %d, local sum: %d\n", id, start, end,
           local_sum);
    result[id] = local_sum;
    return NULL;
}