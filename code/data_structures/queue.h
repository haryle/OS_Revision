#include <pthread.h>

typedef struct ___node_t {
    int value;
    struct ___node_t* next;
} node_t;

typedef struct ___queue_t {
    node_t* head;
    node_t* tail;
    pthread_mutex_t head_lock, tail_lock;
} queue_t;

void Queue_Init(queue_t* Q);

void Queue_Enqueue(queue_t* Q, int key);

void Queue_Dequeue(queue_t* Q, int* key);