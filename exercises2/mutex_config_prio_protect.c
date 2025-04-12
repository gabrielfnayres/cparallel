#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


pthread_mutex_t lock;
pthread_mutexattr_t attr;


int main(){

    pthread_mutexattr_init(&attr);

    pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_PROTECT);

    pthread_mutexattr_setprioceiling(&attr, 10);

    pthread_mutex_init(&lock, &attr);


    pthread_mutex_destroy(&lock);
    pthread_mutex(&attr);
}