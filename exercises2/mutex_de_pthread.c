#include <stdio.h>
#include <pthread.h>


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_function(void *arg){

    pthread_mutex_lock(&mutex);

    printf("Thread %ld entrou na secao critica.\n", pthread_self());
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(){

    pthread_t t1, t2;

    pthread_create(&t1, NULL, thread_function, NULL);
    pthread_create(&t2, NULL, thread_function, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&mutex);
    
    return 0;
}