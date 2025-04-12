#include <stdio.h>
#include <pthread.h>


pthread_mutex_t lock;



int main(){


    pthread_mutex_init(&lock, NULL);


    pthread_mutex_destroy(&lock);
    return 0;

}

