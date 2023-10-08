#include "list.h"

#define BUCKETS 101

typedef struct ___hash_t {
    list_t lists[BUCKETS];
} hash_t;

void Hash_Init(hash_t* H);

int Hash_Insert(hash_t* H, int key);

int Hash_Lookup(hash_t* H, int key);
