#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>


#define NUM_GRUPOS 8

// inicializando semáforo
sem_t sem_cabine;

void*  grupo(void* arg){
    
    // fazendo o casting de tipo para o id que referencia o grupo
    int id = *(int*) arg;

    sem_wait(&sem_cabine); // decrescendo contador do semáforo  com o sinal WAIT

    int t_time = (rand() % 4 + 2); // contabilizando tempo de viagem

    printf("Grupo %d embarcou na cabine. (Tempo de viagem: %ds)\n",id, t_time);
    /// PRINTANDO INFORMACOES INICIAIS

    sleep(t_time); // emulando tempo de viagem com sleep

    printf("Grupo %d desembarcou. Cabine disponível!\n", id); // printando informacoes finais

    sem_post(&sem_cabine); // Fechando ciclo da thread com o sinal de post para incrementar o contador

    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t threads[NUM_GRUPOS];
    int ids[NUM_GRUPOS];
    sem_init(&sem_cabine, 0, 4); // iniciando semáforo com  4 cabines
    for (int i = 0; i < NUM_GRUPOS; i++) {
    ids[i] = i + 1;
    pthread_create(&threads[i], NULL, grupo, &ids[i]);
    }
    for (int i = 0; i < NUM_GRUPOS; i++) {
    pthread_join(threads[i], NULL);
    }
    sem_destroy(&sem_cabine); // liberando semáforo
    return 0;
}