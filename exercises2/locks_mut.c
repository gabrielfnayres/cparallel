#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>


// busy waiting (espera ocupada) técnica de sincronização de sistemas concorrentes
// onde uma thread ou processa verifica reptidamente uma condição até que ela seja satisfeita
// a cpu fica ocupada ao invés de realizar algo util


bool condition_met  = false;

void acquire_condition(){
    while(!condition_met){

    }
}


/// travas ( com espera ocupada)

volatile bool lock =  false;

void acquire_lock(){

    while(__sync_lock_test_and_set(&lock, true)){
        // espera ocupada enquanto lock for true
    }

}
void release_lock(){
    __sync_lock_release(&lock);
    // libera o lock 
}

int shared_counter = 0;

void* thread_function(void* arg){

    for(int i = 0; i < 1000000; i++){
        acquire_lock();
        shared_counter++;
        release_lock();
    }
    return NULL;
}


int main(){

    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, thread_function, NULL);
    pthread_create(&thread2, NULL, thread_function, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("shared_counter = %d\n", shared_counter);
    return 0;
}


