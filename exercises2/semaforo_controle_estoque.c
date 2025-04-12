#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <time.h>


#define NUM_PRODUTOS 5
#define CAPACIDADE_MAXIMA 100
#define NUM_PRODUTORES 3
#define NUM_CONSUMIDORES 5
#define NUM_OPERACOES 20


typedef struct{
    int id;

    char nome[50];
    int quantidade;
    float preco;
    sem_t semaforo;
} produto_t;

typedef struct{
    produto_t produtos[NUM_PRODUTORES];
    sem_t mutex_relatorio;
    sem_t mutex_log;
}estoque_t;

typedef struct{
    int id;
    estoque_t* estoque;
}args_thread_t;

