#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdatomic.h>


#define NUM_THREADS 5

atomic_int barrier_counter = 0;

int total_thread = NUM_THREADS;

void barrier_wait(){
  atomic_fetch_add(&barrier_counter, 1);

  while(barrier_counter < total_thread){

  }
}



void *thread_func(void *arg){
  int thread_id = *((int*) arg);
  printf("Thread %d: Executing before barrier\n", thread_id);
  sleep(1);
  printf("Thread %d getting close to the barrier\n", thread_id);  
  barrier_wait();

  printf("Thread %d: out of the barrier\n",thread_id);
  return NULL;
}

int main(){
  pthread_t threads[NUM_THREADS];

  int thread_ids[NUM_THREADS];


  for(int i = 0; i < NUM_THREADS; i++){
    thread_ids[i] = i + 1;
    pthread_create(&threads[i], NULL, thread_func, &thread_ids[i]);
  }

  for (int i = 0; i < NUM_THREADS; i++){
    pthread_join(threads[i], NULL);
  }
  printf("Todas as threads passaram pela barreira.\n");
  return 0;
}
