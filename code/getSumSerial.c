#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#ifndef MAXTHREAD
    #define MAXTHREAD 4
#endif

#ifndef N
    #define N 1000
#endif

int main(){
    long long int result = 0;
    for (int i = 0; i < N; i++){
        result += i;
    }
    printf("%lld\n", result);
}