#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <unistd.h>


#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];

int cont = 0;

atomic_flag lock = ATOMIC_FLAG_INIT;

void acquire_lock(){
    while(atomic_flag_test_and_set(&lock)){
        // espera ocupada
    }
}

void release_lock(){
    atomic_flag_clear(&lock);
}

void *produtor(void *args){

    for(int i = 0; i< 20; i++){
        usleep(100000);
        int item = i + 1;
        acquire_lock();
        if(cont < BUFFER_SIZE){
            buffer[cont] = item;
            cont++;
            printf("Produtor: Produziu item %d\n", item);        
        }else{
            printf("Produto: Buffer cheio, item %d não produzido\n", item);
        }
        release_lock();
    }
    return NULL;
}

void *consumidor(void *args){

    for(int i = 0; i < 20; i++){

        acquire_lock();
        if(cont > 0){

            int item = buffer[cont - 1];
            cont--;
            printf("Consumidor: Consumiu item %d\n", item);

        }else{
            printf("Consumidor: Buffer vazio, nada para consumir\n");
        }
        release_lock(); 
        usleep(150000);
    }
    return NULL;
}

int main(){

    pthread_t t1, t2;


    pthread_create(&t1, NULL, produtor, NULL);
    pthread_create(&t2, NULL, consumidor, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}