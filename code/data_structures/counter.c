#include "counter.h"

void Counter_Init(counter_t* C, int threshold) {
    C->threshold = threshold;
    C->global = 0;
    pthread_mutex_init(&C->global_lock, NULL);
    int i;
    for (i = 0; i < CPUCOUNT; i++) {
        C->local[i] = 0;
        pthread_mutex_init(&C->local_lock[i], NULL);
    }
}

void Counter_Update(counter_t* C, pthread_t threadID, int amount) {
    int id = threadID % CPUCOUNT;
    pthread_mutex_lock(&C->local_lock[id]);
    C->local[id] += amount;
    if (C->local[id] >= C->threshold) {
        pthread_mutex_lock(&C->global_lock);
        C->global += C->local[id];
        pthread_mutex_unlock(&C->global_lock);
        C->local[id] = 0;
    }
    pthread_mutex_unlock(&C->local_lock[id]);
}

void Counter_Dump(counter_t* C){
    int i = 0;
    for (i = 0; i < CPUCOUNT; i++){
        pthread_mutex_lock(&C->local_lock[i]);
        if (C->local[i] > 0){
            pthread_mutex_lock(&C->global_lock);
            C->global += C->local[i];
            pthread_mutex_unlock(&C->global_lock);
            C->local[i] = 0;  
        }
        pthread_mutex_unlock(&C->local_lock[i]);
    }
}

int Counter_Get(counter_t* C) {
    pthread_mutex_lock(&C->global_lock);
    int retval = C->global;
    pthread_mutex_unlock(&C->global_lock);
    return retval;
}