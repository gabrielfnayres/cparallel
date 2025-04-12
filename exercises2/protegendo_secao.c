#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int cont  = 0;


void* thread_function(void* arg){

    pthread_mutex_lock(&mutex);

    cont++;
    printf("Thread %ld esta com cont = %d\n", pthread_self(), cont);

    pthread_mutex_unlock(&mutex);

}


int main(){

    pthread_t threads[5];

    for(int i = 0; i < 5; i++){

        pthread_create(&threads[i], NULL, thread_function, NULL);
    }

    for(int i = 0; i < 5; i++){
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("final counter : %d", cont );
    return 0;
}