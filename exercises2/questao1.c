#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>



typedef struct {
    int id;
    char* nome;
    float saldo;
} conta;
