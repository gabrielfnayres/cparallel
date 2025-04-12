#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


#define MAX_TASKS 10
#define NUM_CLIENT_THREADS 5
#define NUM_SERVER_THREADS 2

typedef struct {
    int task_queue[MAX_TASKS];
    int task_count;
    pthread_mutex_t mutex;
} TaskQueue;

TaskQueue queue;

void add_task(TaskQueue* q, int task){

    if(q->task_count < MAX_TASKS){
        q->task_queue[q->task_count] = task;
        q->task_count++;
        printf("tarefa %d adicionada na fila\n", task);
    }else{
        printf("FILA CHEIA\n");
    }
}

void process_task(TaskQueue* q){
    if(q->task_count > 0){
        int task = q->task_queue[0];
        for(int i = 1; i < q->task_count; i++){
            q->task_queue[i - 1] = q->task_queue[i];
        }
        q->task_count--;
        printf("Tarefa %d processada\n", task);
    }else{
        printf("Nenhuma tarefa processada\n");
    }
}


void* client_thread_func(void* arg){

    int client_id = *(int*)arg;
    int task_id = client_id*100;

    while(1){
        if(pthread_mutex_trylock(&queue.mutex)== 0){
            printf("Cliente %d: Mutex adquirido. Adicionando tarefa...\n", client_id);
            add_task(&queue, task_id);
            pthread_mutex_unlock(&queue.mutex);
            printf("Cliente %d: Mutex liberado\n", task_id);
        }else{
            printf("Mutex %d ocupado, realizando outra operacao...\n", task_id);
            sleep(1);
        }
        sleep(rand() %3 + 1);
    }
    return NULL;
}

void* server_thread_func(void* arg){
    int server_id = *(int*) arg;

    while(1){
        if(pthread_mutex_trylock(&queue.mutex) == 0){
            printf("Server %d : Mutex Adquirido. Processando tarefa...\n", server_id);
            process_task(&queue);
            pthread_mutex_unlock(&queue.mutex);
            printf("Server %d: mutex liberado\n", server_id);
        }else{
            printf("Mutex %d ocupado...\n", server_id);
            sleep(1);
        }
        sleep(rand() % 3 + 1);
    }
    return NULL;
}

int main(){

    pthread_t clients[NUM_CLIENT_THREADS];
    pthread_t servers[NUM_SERVER_THREADS];

    int clientids[NUM_CLIENT_THREADS];
    int serverids[NUM_SERVER_THREADS];

    queue.task_count = 0;
    pthread_mutex_init(&queue.mutex, NULL);

    for(int i = 0; i < NUM_CLIENT_THREADS; i++){
        clientids[i] = i + 1;
        pthread_create(&clients[i], NULL, client_thread_func, &clientids[i]);
    }

    for(int i = 0; i < NUM_SERVER_THREADS; i++){
        serverids[i] = i + 1;
        pthread_create(&servers[i], NULL, server_thread_func, &serverids[i]);
    }

    for(int i = 0; i < NUM_CLIENT_THREADS; i++){
        pthread_join(clients[i], NULL);
    }

    for(int i = 0; i < NUM_SERVER_THREADS; i++){
        pthread_join(servers[i], NULL);
    }

    pthread_mutex_destroy(&queue.mutex);
    return 0;

}