#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#define NUM_THREADS 8

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



#define MAT_SIZE 1024

typedef struct Mat_args{
  int id;
  int start_row;
  int end_row;
} Mat_args;


int a[MAT_SIZE][MAT_SIZE], b[MAT_SIZE][MAT_SIZE], c[MAT_SIZE][MAT_SIZE];

pthread_barrier_t barrier;

void* mul(void* arg){

  Mat_args* aux = (Mat_args*) arg;

  for(int k = 0; k < MAT_SIZE; k++){
    for(int i = aux->start_row; i < aux->end_row; i++){
      for(int j = 0; j < MAT_SIZE; j++){  
        a[i][j] += b[i][k]*c[k][j];
      }
    }
    printf("at the barrier %d, id: %d\n", k, aux->id);
    pthread_barrier_wait(&barrier);
    printf("after the barrier %d, id: %d\n", k, aux->id);

  }
  
  return NULL;
}

int main(){

  pthread_barrier_init(&barrier, NULL, NUM_THREADS);

  pthread_t td[NUM_THREADS];
  Mat_args args[NUM_THREADS];

  int row_per_thread = MAT_SIZE/NUM_THREADS;

  for(int i = 0; i < MAT_SIZE; i++){
    for(int j = 0; j < MAT_SIZE; j++){
      c[i][j] = (i == j) ? 1.0 : 0.0; // Matriz identidade
      b[i][j] = 1;
    }
  }

  for(int i = 0; i < NUM_THREADS; i++){
    args[i].id = i + 1;
    args[i].start_row = i *row_per_thread;

    args[i].end_row = (i == NUM_THREADS - 1) ? MAT_SIZE : (i + 1) * row_per_thread;
    pthread_create(&td[i], NULL, mul, &args[i]);
  }

  for(int i = 0; i < NUM_THREADS; i++){
    pthread_join(td[i], NULL);
  }

  pthread_barrier_destroy(&barrier);

  printf("C[0][0] = %d\n", a[0][0]); // Deve ser 1.0 * 1.0 * N = N
  return 0;
}
