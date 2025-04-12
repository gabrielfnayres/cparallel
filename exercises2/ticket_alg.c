#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <unistd.h>

#define NUM_THREADS 5

volatile atomic_int next_ticket = 0;
volatile atomic_int now_serving = 0;

void acquire_lock(int* my_ticket){
    *my_ticket = atomic_fetch_add(&next_ticket, 1);
    while(atomic_load(&now_serving) != *my_ticket){
        
    }
}

void release_lock(){
    atomic_fetch_add(&now_serving, 1);  
}

int shared_counter = 0;

void* thread_function(void* arg){
    int my_ticket;
    acquire_lock(&my_ticket);

    shared_counter++;
    printf("Thread %ld incrementou shared counter para %d\n", pthread_self(),my_ticket);
    release_lock();
    return NULL;
}

int main(){

    pthread_t threads[NUM_THREADS];

    for(int i = 0; i < NUM_THREADS; i++){
        pthread_create(&threads[i], NULL, thread_function, NULL);
    }

    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    printf("shared_counter = %d\n", shared_counter);
    return 0;
}

    