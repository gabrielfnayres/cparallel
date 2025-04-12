#include <stdio.h>
#include <pthread.h>


pthread_mutex_t lock; // mutex
pthread_mutexattr_t attr; // atributos


void *thread_function(void *arg){

    pthread_mutex_lock(&lock);
    printf("Thread %ld entrou na secao critica\n",  (long) arg);
    pthread_mutex_unlock(&lock);
    return NULL;
}


int main(){
    pthread_t t1, t2;

    pthread_mutexattr_init(&attr);

    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);


    pthread_mutex_init(&lock, &attr);

    pthread_create(&t1, NULL, thread_function, (void*) 1);
    pthread_create(&t2, NULL, thread_function, (void*)2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);
    pthread_mutexattr_destroy(&attr);

    return 0;
}
