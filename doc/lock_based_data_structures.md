## Concurrent Counter

### Non thread-safe implementation 

```C
typedef struct __counter_t{
    int value;
} counter_t;

void init(counter_t *c){
    c->value = 0;
}

void increment(counter_t *c){
    c->value++;
}

void decrement(counter_t *c){
    c->value--;
}

int get(counter_t *c){
    return c->value;
}
```

The problem with this approach is race condition. Hence not thread-safe.

### Thread-safe with global lock 

```C
typedef struct __counter_t{
    int value;
    pthread_mutex_t counter_lock;
} counter_t;

void init(counter_t *c){
    c->value = 0;
    pthread_mutex_init(&c->counter_lock, NULL);
}

void increment(counter_t *c){
    pthread_mutex_lock(&c->counter_lock);
    c->value++;
    pthread_mutex_unlock(&c->counter_lock);
}

void decrement(counter_t *c){
    pthread_mutex_unlock(&c->counter_lock);
    c->value--;
    pthread_mutex_unlock(&c->counter_lock);
}

int get(counter_t *c){
    pthread_mutex_unlock(&c->counter_lock);
    int retval = c->value;
    pthread_mutex_unlock(&c->counter_lock);
    return retval;
}
```

The disadvantage of this global lock is it being slow. 

## Thread-safe faster counter with approximation 

```C
typedef struct ___counter_t{
    int global;
    int local[CPU_COUNT];
    pthread_mutex_t global_lock;
    pthread_mutex_t local_lock[CPU_COUNT];
    int threshold;
}counter_t;

void init(counter_t *c, int threshold){
    c->threshold = threshold;
    c->global = 0;
    pthread_mutex_init(&c->global_lock, NULL);
    int i; 
    for (i = 0; i < CPU_COUNT; i++){
        c->local[i] = 0;
        pthread_mutex_init(&c->local_lock, NULL);
    }
}

void update(counter_t *c, int amount, int threadID){
    int id = threadID % CPU_COUNT;
    pthread_mutex_lock(c->local_lock[id]);
    c->local[id] += amount;
    if (c->local[id] >= c->threshold){
        pthread_mutex_lock(c->global_lock);
        c->global+= c->local[id];
        pthread_mutex_unlock(c->global_lock);
        c->local[id] = 0; 
    }
    pthread_mutex_unlock(c->local_lock[id]);
}

int get(counter_t *c){
    pthread_mutex_lock(c->global_lock);
    int retval = c->global;
    pthread_mutex_unlock(c->global_lock);
    return retval;
}
```

## Concurrent Linked List 

```C
typedef struct __node_t{
    int key;
    struct __node_t *next;
} node_t;

typedef struct __list_t{
    node_t *head;
    pthread_mutex_lock lock;
} list_t;

void List_Init(list_t *L){
    L->head = NULL;
    pthread_mutex_init(&L->lock);
}

int List_Insert(list_t *L, int key){
    // No lock held while setting up malloc 
    // To prevent common bug 
    // Synchronisation here not needed 
    node_t* new_ptr = (node_t*) malloc(sizeof(node_t));

    if (new_ptr == NULL){
        perror("malloc");
        return -1;
    }
    new_ptr->key = key;
    // Just lock the critical section
    pthread_mutex_lock(&L->lock);
    new_ptr->next = L->head;
    L->head = new_ptr;
    pthread_mutex_unlock(&L->lock);
    return 0;
}

int List_Lookup(list_t *L, int key){
    pthread_mutex_lock(&L->lock);
    node_t * current = L->head;
    while (current != NULL){
        if (current->key == key){
            pthread_mutex_unlock(&L->lock);
            return 0;
        }
        current = current->next;
    }
    pthread_mutex_unlock(&L->lock);
    return -1;
}
```

## Concurrent HashTable

```C
#define BUCKET 100;

typedef struct ___hash_t {
    list_t lists[BUCKET];
} hash_t;

void Hash_Init(hash_t *H){
    int i;
    for (i = 0; i < BUCKET; i++){
        List_Init(&H->lists[i]);
    }
}

int Hash_Insert(hash_t *H, int key){
    return List_Insert(&H->lists[key % BUCKET], key);
}

int Hash_Lookup(hash_t *H, int key){
    return List_Lookup(&H->lists[key % BUCKET], key);
}

```

Thread safety is provided by the data structure concurrent linked list.

## Concurrent Queues

```C
typedef struct ___node_t{
    int value;
    struct ___node_t *next;
} node_t;

typedef struct ___queue_t{
    node_t *head;
    node_t *tail;
    pthread_mutex_t head_lock, tail_lock;
} queue_t;

void Queue_Init(queue_t* Q){
    node_t *temp = (node_t*) malloc(sizeof(node_t));
    temp->next = NULL;
    Q->head = Q->tail = temp;
    pthread_mutex_init(&Q->head_lock, NULL);
    pthread_mutex_init(&Q->tail_lock, NULL);
}

void Queue_Enqueue(queue_t* Q, int value){
    node_t *new = (node_t*) malloc(sizeof(node_t));
    assert(new!=NULL);
    new->value=value;
    new->next=NULL;
    pthread_mutex_lock(&Q->tail_lock);
    Q->tail->next = new;
    Q->tail = new;
    pthread_mutex_unlock(&Q->tail_lock);
}

int Queue_Dequeue(queue_t* Q, int* value){
    pthread_mutex_lock(&Q->head_lock);
    node_t *tmp = Q->head;
    node_t *new_head = Q->head->next;
    if (new_head == NULL){
        pthread_mutex_unlock(&Q->head_lock);
        return -1; // Queue is empty
    }
    Q->head = new_head;
    tmp->value = value;
    pthread_mutex_unlock(&Q->head_lock);
    free(tmp);
    return 0; 
}

```