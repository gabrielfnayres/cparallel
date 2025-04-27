#include <stddef.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/_pthread/_pthread_cond_t.h>
#include <sys/_pthread/_pthread_mutex_t.h>
#include <unistd.h>
#include <errno.h>
#define NUM_THREADS 10

#ifdef __APPLE__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PTHREAD_BARRIER_SERIAL_THREAD
# define PTHREAD_BARRIER_SERIAL_THREAD -1
#endif

typedef pthread_mutexattr_t pthread_barrierattr_t;

/* structure for internal use that should be considered opaque */
typedef struct {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	unsigned count;
	unsigned left;
	unsigned round;
} pthread_barrier_t;

int pthread_barrier_init(pthread_barrier_t *__restrict barrier,
                         const pthread_barrierattr_t * __restrict attr,
                         unsigned count);
int pthread_barrier_destroy(pthread_barrier_t *barrier);

int pthread_barrier_wait(pthread_barrier_t *barrier);

int pthread_barrierattr_init(pthread_barrierattr_t *attr);
int pthread_barrierattr_destroy(pthread_barrierattr_t *attr);
int pthread_barrierattr_getpshared(const pthread_barrierattr_t *__restrict attr,
                                   int *__restrict pshared);
int pthread_barrierattr_setpshared(pthread_barrierattr_t *attr,
                                   int pshared);

int pthread_barrier_init(pthread_barrier_t *__restrict barrier,
                         const pthread_barrierattr_t * __restrict attr,
                         unsigned count) {
	if (count == 0) {
		return EINVAL;
	}

	int ret;

	pthread_condattr_t condattr;
	pthread_condattr_init(&condattr);
	if (attr) {
		int pshared;
		ret = pthread_barrierattr_getpshared(attr, &pshared);
		if (ret) {
			return ret;
		}
		ret = pthread_condattr_setpshared(&condattr, pshared);
		if (ret) {
			return ret;
		}
	}

	ret = pthread_mutex_init(&barrier->mutex, attr);
	if (ret) {
		return ret;
	}

	ret = pthread_cond_init(&barrier->cond, &condattr);
	if (ret) {
		pthread_mutex_destroy(&barrier->mutex);
		return ret;
	}

	barrier->count = count;
	barrier->left = count;
	barrier->round = 0;

	return 0;
}

int pthread_barrier_destroy(pthread_barrier_t *barrier) {
	if (barrier->count == 0) {
		return EINVAL;
	}

	barrier->count = 0;
	int rm = pthread_mutex_destroy(&barrier->mutex);
	int rc = pthread_cond_destroy(&barrier->cond);
	return rm ? rm : rc;
}


int pthread_barrier_wait(pthread_barrier_t *barrier) {
	pthread_mutex_lock(&barrier->mutex);
	if (--barrier->left) {
		unsigned round = barrier->round;
		do {
			pthread_cond_wait(&barrier->cond, &barrier->mutex);
		} while (round == barrier->round);
		pthread_mutex_unlock(&barrier->mutex);
		return 0;
	} else {
		barrier->round += 1;
		barrier->left = barrier->count;
		pthread_cond_broadcast(&barrier->cond);
		pthread_mutex_unlock(&barrier->mutex);
		return PTHREAD_BARRIER_SERIAL_THREAD;
	}
}


int pthread_barrierattr_init(pthread_barrierattr_t *attr) {
	return pthread_mutexattr_init(attr);
}

int pthread_barrierattr_destroy(pthread_barrierattr_t *attr) {
	return pthread_mutexattr_destroy(attr);
}

int pthread_barrierattr_getpshared(const pthread_barrierattr_t *__restrict attr,
                                   int *__restrict pshared) {
	return pthread_mutexattr_getpshared(attr, pshared);
}

int pthread_barrierattr_setpshared(pthread_barrierattr_t *attr, int pshared) {
	return pthread_mutexattr_setpshared(attr, pshared);
}
#ifdef  __cplusplus
}
#endif

#endif /* __APPLE__ */




pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

pthread_barrier_t barrier;

int cont = 0;
int round_ = 0;


void* thread_function(void * arg){

  int id = *((int*)arg);
  printf("Thread %d, initializing phase 1\n", id);

  sleep(id);
  int my_round = round_;
  cont++;

  pthread_barrier_wait(&barrier);

  if(cont == NUM_THREADS){
    cont = 0;
    round_++;
    printf("Thread %d ultima a chegar na barreira 1\n", id);
  }

  sleep(id);

  my_round = round_;
  cont++;

  pthread_barrier_wait(&barrier);

  if(cont == NUM_THREADS){
    cont = 0;
    round_++;
    printf("Thread %d ultima a chegar na barreira 2\n", id);
  }


  printf("fim\n");

  return NULL;
}


int main(){
  pthread_t td[NUM_THREADS];
  int ids[NUM_THREADS];
  pthread_barrier_init(&barrier, NULL, NUM_THREADS);


  for(int i = 0; i < NUM_THREADS; i++){
    pthread_create(&td[i], NULL, thread_function, &ids[i]);
  }

  for(int i = 0; i < NUM_THREADS; i++){
    pthread_join(&td[i], NULL);
  }

  pthread_barrier_destroy(&barrier);

  return 0;
}
