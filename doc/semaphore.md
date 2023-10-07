## Semaphore 

Semaphore is an object with integer value. 

- `sem_wait`
- `sem_post`

Before calling other routines, a semaphore value must first be initialised:

```C
#include <semaphore.h>
sem_t s;
// The second parameter indicates semaphore is shared between all threads in the same process
sem_init(&s, 0 , 1); // initial value set ot 1

sem_wait(&m);
// critical section
sem_post(&m); 
```

`sem_wait` semantic: decrease semaphore value by one and wait if value is negative. `sem_wait` may return immediately if semaphore value is positive. Otherwise it will wait for subsequent post. When semaphore value is negative, it is equal to the number of waiting threads. 

`sem_post` semantic: increase semaphore value by one and if there are one or more threads waiting, wake one. 
