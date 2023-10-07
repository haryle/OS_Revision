#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "data_structures/list.h"

#ifndef CPUCOUNT
    #define CPUCOUNT 4
#endif

#define VALUE 1000
list_t list;

void* thread_insert(void* arg) {
    int* argv= (int* )arg;
    int cpuID = *argv;
    int i; 
    int start = cpuID * VALUE / CPUCOUNT;
    int end = (cpuID + 1) * VALUE / CPUCOUNT;
    for (i = start; i < end; i++)
        List_Insert(&list, i);
    printf("Thread id: %d, start: %d, end: %d\n", cpuID, start, end);
    return NULL;
}

int main() {
    printf("CPUCOUNT: %d\n", CPUCOUNT);
    List_Init(&list);
    pthread_t thr[CPUCOUNT];
    int i = 0;
    int id[CPUCOUNT]; 
    for (i = 0; i < CPUCOUNT; i++){
        id[i] = i; 
    }
    for (i = 0; i < CPUCOUNT; i++) {
        pthread_create(&thr[i], NULL, thread_insert, (void*) &(id[i]));
    }
    for (i = 0; i < CPUCOUNT; i++) {
        pthread_join(thr[i], NULL);
    }
    for (i = 0; i < VALUE; i++){
        if (List_Lookup(&list, i) !=0){
            printf("Error: cannot find: %d\n", i);
            return -1;
        }
    }
    printf("Success\n");
    return 0; 
}