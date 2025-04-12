#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


#define NUM_ASSENTOS 10

int assentos[NUM_ASSENTOS] = {0};
pthread_mutex_t lock;


void* reserva_assentos(void* arg){

    int id = (int) arg;

    for(int i = 0; i < NUM_ASSENTOS; i++){
        pthread_mutex_lock(&lock);

        if(assentos[i] == 0){
            assentos[i] = 1;
            printf("Cliente %d: Reservou assento %d\n", id, i + 1);
            pthread_mutex_unlock(&lock);
            break;
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}


int main(){

    pthread_t clientes[NUM_ASSENTOS + 2];
    int ids[NUM_ASSENTOS + 2];
    
    pthread_mutex_init(&lock, NULL);
    for(int i = 0; i < NUM_ASSENTOS + 2; i++){
        ids[i]=i+1;
        pthread_create(&clientes[i], NULL, reserva_assentos, NULL);
    }

    for(int i = 0; i < NUM_ASSENTOS + 2; i++){
        pthread_join(clientes[i], NULL);
    }


    pthread_mutex_destroy(&lock);


    printf("Estado final dos assentos: ");
    for (int i = 0; i < NUM_ASSENTOS; i++) {
        printf("%d ", assentos[i]);
    }
    printf("\n");

}



