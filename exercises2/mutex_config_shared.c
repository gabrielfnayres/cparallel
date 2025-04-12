#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/_pthread/_pthread_mutexattr_t.h>


// memoria compartilhada 



typedef struct {
    pthread_mutex_t mutex;
    int counter;
}shared_data_t;


int main(){

    const char *shm_name = "/shm_example";
    int shm_fd;
    shared_data_t *shared_data;

    size_t shared_size = sizeof(shared_data_t);

    shm_fd = shm_open(shm_name,O_CREAT | O_RDWR, 0666);

    if(shm_fd == -1){
        perror("Erro ao criar memória compartilhada\n");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shm_fd, shared_size)==-1){
        perror("Erro ao definir o tamanho da memoria compartilahda\n");
        exit(EXIT_FAILURE);
    }

    // mapeia memoria 
    shared_data = mmap(NULL, shared_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(shared_data == MAP_FAILED){
        perror("erro ao mapear memoria\n");
        exit(EXIT_FAILURE);
    }

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_PROCESS_SHARED);

    pthread_mutex_init(&shared_data->mutex, &attr);
    pthread_mutexattr_destroy(&attr);

    shared_data->counter = 0;


    // cria processo filho
    pid_t pid = fork();
    if(pid == -1){

        perror("erro ao criar processo filho\n");
        exit(EXIT_FAILURE);
    }

    if(pid == 0){
        // processo filho
        
        for(int i = 0; i < 10; i++){
            pthread_mutex_lock(&shared_data->mutex);
            shared_data->counter++;
            printf("Processo filho incrementando o contador para %d\n", shared_data->counter);
            pthread_mutex_unlock(&shared_data->mutex);
            sleep(1);    
        }
    }else{
        for(int i = 0; i < 10; i++){
            pthread_mutex_lock(&shared_data->mutex);
            shared_data->counter++;
            printf("Processo pai incrementando para %d\n", shared_data->counter);
            pthread_mutex_unlock(&shared_data->mutex);
            sleep(1);
        }
     
        //espera processo filho terminar
        wait(NULL);

        // limpa tudo

        pthread_mutex_destroy(&shared_data->mutex);
        munmap(shared_data, shared_size);
        shm_unlink(shm_name);
    }
    return 0;

}