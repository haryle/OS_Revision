#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

void Queue_Enqueue(queue_t* Q, int key) {
    node_t* tmp = (node_t*)malloc(sizeof(node_t));
    if (tmp == NULL) {
        perror("malloc");
        return -1;
    }
    tmp->value = key;
    tmp->next = NULL;
    pthread_mutex_lock(&Q->tail_lock);
    Q->tail->next = tmp;
    Q->tail = tmp;
    pthread_mutex_unlock(&Q->tail_lock);
    return 0;
}

void Queue_Dequeue(queue_t* Q, int* key) {
    pthread_mutex_lock(&Q->head_lock);
    node_t* tmp = Q->head;
    node_t* new_head = Q->head->next;
    if (new_head == NULL) {
        pthread_mutex_unlock(&Q->head_lock);
        return -1;  // List is empty
    }
    *key = tmp->value;
    Q->head = new_head;
    pthread_mutex_unlock(&Q->head_lock);
    free(tmp);
    return 0;
}