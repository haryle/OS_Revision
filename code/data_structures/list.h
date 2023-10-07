#include <pthread.h>

typedef struct ___node_t {
    int value;
    struct ___node_t* next;
} node_t;

typedef struct ___list_t{
    node_t* head;
    pthread_mutex_t lock;
}list_t; 

void List_Init(list_t* L);

int List_Insert(list_t*L, int key);

int List_Lookup(list_t*L, int key);