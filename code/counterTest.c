#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "data_structures/counter.h"
#define THRESHOLD 100
#define VALUE 1000
counter_t counter;

void* thread_count(void* arg) {
    pthread_t id = pthread_self();
    int* argv = (int*)arg;
    int i = 0;
    for (i = 0; i < *argv; i++)
        Counter_Update(&counter, id, 1);
    return NULL;
}

int main() {
    printf("CPUCOUNT: %d\n", CPUCOUNT);
    Counter_Init(&counter, THRESHOLD);
    pthread_t thr[CPUCOUNT];
    int amount = ((int)VALUE) / ((int)CPUCOUNT);
    int i = 0;
    for (i = 0; i < CPUCOUNT; i++) {
        pthread_create(&thr[i], NULL, thread_count, (void*)&amount);
    }
    for (i = 0; i < CPUCOUNT; i++) {
        pthread_join(thr[i], NULL);
    }

    Counter_Dump(&counter);
    printf("%d\n", Counter_Get(&counter));
}