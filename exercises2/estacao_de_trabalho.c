#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <unistd.h>


#define NUM_USERS 5


atomic_int next_ticket = 0;
atomic_int now_serving = 0;


void acquire_lock(){

    int my_ticket = atomic_fetch_add(&next_ticket, 1);
    while(atomic_load(&now_serving) != my_ticket){
        // espera ocupada
    }
}

void release_lock(){
    atomic_fetch_add(&now_serving, 1);
}


void *usar_estacao(void *arg){
    int id = (int) arg;
    acquire_lock();
    printf("Usuario %d, acessando a estacao de trabalho\n", id);
    usleep(1000000);
    printf("Usuario %d liberando estacao de trabalho\n", id);
    release_lock();
    return NULL;
}

int main(){

    pthread_t threads[NUM_USERS];
    int ids[NUM_USERS];

    for(int i = 0; i < NUM_USERS; i++){
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, usar_estacao, &ids[i]);
    }

    for(int i = 0; i < NUM_USERS; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}