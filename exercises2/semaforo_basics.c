#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 4
#define ITERATIONS 1000000

long contador = 0;

sem_t mutex;

void *incrementar(void* arg){

    int id = *(int*)arg;
    
    for(int i = 0; i < ITERATIONS; i++){
        sem_wait(&mutex);

        contador++;

        sem_post(&mutex);
    }

    printf("Thread %d concluida\n", id);
    return NULL;
}

int main(){

    pthread_t t[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    sem_init(&mutex, 0, 1);

    for(int i = 0; i < NUM_THREADS; i++){

        thread_ids[i] = i + 1;

        pthread_create(&t[i], NULL, incrementar, &thread_ids[i]);
    }

    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(&t[i], NULL);
    }

    sem_destroy(&mutex);

    printf("Valor esperado: %ld\n", (long)NUM_THREADS * ITERATIONS);
    printf("Valor obtido: %ld\n", contador);

    return 0;
}
