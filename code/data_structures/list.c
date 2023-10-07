#include "list.h"
#include <stdlib.h> 
#include <stdio.h> 

void List_Init(list_t* L) {
    L->head = NULL;
    pthread_mutex_init(&L->lock, NULL);
}

int List_Insert(list_t* L, int key) {
    node_t* tmp = (node_t*)malloc(sizeof(node_t));
    if (tmp == NULL) {
        perror("malloc\n");
        return 0;
    }
    tmp->value = key;
    pthread_mutex_lock(&L->lock);
    tmp->next = L->head;
    L->head = tmp;
    pthread_mutex_unlock(&L->lock);
    return 1;
}

int List_Lookup(list_t* L, int key) {
    pthread_mutex_lock(&L->lock);
    node_t* current = L->head;
    while (current != NULL) {
        if (current->value == key) {
            pthread_mutex_unlock(&L->lock);
            return 0;
        }
        current = current->next;
    }
    pthread_mutex_unlock(&L->lock);
    return 1;
}
