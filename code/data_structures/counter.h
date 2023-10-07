#include <pthread.h>
#ifndef CPUCOUNT
    #define CPUCOUNT 4
#endif

typedef struct ___counter_t{
    int global;
    int local[CPUCOUNT];
    pthread_mutex_t global_lock;
    pthread_mutex_t local_lock[CPUCOUNT];
    int threshold;
}counter_t;

void Counter_Init(counter_t* C, int threshold);

void Counter_Dump(counter_t* C);

void Counter_Update(counter_t* C, pthread_t threadID, int amount);

int Counter_Get(counter_t* C);