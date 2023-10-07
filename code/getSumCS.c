/*
Use pthreads to calculate sum of a large array. 
Local sum is written to a global variable called result.
Hoang Son Le a1691819 7/10/20203
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef MAXTHREAD
    #define MAXTHREAD 4
#endif

#ifndef N
    #define N 1000
#endif

int array[N];
int params[MAXTHREAD]; 

void init_array(int array[], int size){
    for (int i = 0; i < size; i++)
        array[i] = i; 
}

long long int global_result = 0; 
pthread_mutex_t global_result_lock = PTHREAD_MUTEX_INITIALIZER;

void* thread_sum(void* args);

int main() {
    init_array(array, N);
    init_array(params, MAXTHREAD);
    pthread_t threads[MAXTHREAD];

    for (int i = 0; i < MAXTHREAD; i++) 
        pthread_create(&threads[i], NULL, thread_sum, (void*) &params[i]);


    for (int i = 0; i < MAXTHREAD; i++)
        pthread_join(threads[i], NULL);

    printf("Global result: %lld\n", global_result);

    return 0;
}

void* thread_sum(void* arg) {
    int id = *(int*)arg;
    int start = id * N / MAXTHREAD;
    int end = (id + 1) * N / MAXTHREAD;
    long long int local_sum = 0;
    for (int i = start; i < end; i++)
        local_sum += array[i];
    printf("Thread id: %d, start: %d, end: %d, local sum: %lld\n", id, start, end,
           local_sum);
    pthread_mutex_lock(&global_result_lock);
    global_result += local_sum;
    pthread_mutex_unlock(&global_result_lock);
    return NULL;
}