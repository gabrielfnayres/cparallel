#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define CACHE_SIZE 10
#define NUM_THREADS 4
#define NUM_OPERATIONS 10000
#define READ_RATIO 0.7  // 70% reads, 30% writes

// Spinlock variable
int spinlock = 0;

// Cache structure
/*
    Cache structure based on the key-value pair
*/
typedef struct {
    int key;
    int value;
} cache_entry;

cache_entry cache[CACHE_SIZE];

// Spinlock functions
void spinlock_init() {
    /*
        Initialize the spinlock to 0
    */
    spinlock = 0;
}

int spinlock_lock() {
    /*
        Lock the spinlock
    */
    while (__sync_lock_test_and_set(&spinlock, 1)) {
        // Busy-wait until the lock is available
    }
    return 0;
}

void spinlock_unlock() {
    /*
        Unlock the spinlock with the sync library
    */
    __sync_lock_release(&spinlock);
}

// Cache functions
void cache_init() {
    /*
        Initialize the cache with default values from 0 to CACHE_SIZE(10) 
    */
    for (int i = 0; i < CACHE_SIZE; i++) {
        cache[i].key = i;  // Using index as key
        cache[i].value = 0;
    }
}

int cache_read(int key) {
    /*
        Read a value from the cache type based on the key
    */
    if (key >= 0 && key < CACHE_SIZE) {
        return cache[key].value;
    }
    return -1;  // Invalid key
}

void cache_write(int key, int value) {
    /*
        Write a value to the cache type based on the key
    */
    if (key >= 0 && key < CACHE_SIZE) {
        cache[key].value = value;
    }
}

// Thread function
void* thread_function(void* arg) {
    /*
        Thread function to perform read and write operations on the cache
    */
    int thread_id = *(int*)arg;
    int reads = 0;
    int writes = 0;

    for (int i = 0; i < NUM_OPERATIONS; i++) {
        int key = rand() % CACHE_SIZE; // generate the key limited to the cache size 
        int value = rand() % 100; // generate the value limited to 100
        int operation = ((double)rand() / RAND_MAX < READ_RATIO) ? 0 : 1; // 0 for read, 1 for write

        spinlock_lock();   // Locking the cache for reading or writing


        if (operation == 0) {
            // Perform a read operation
            cache_read(key);
            reads++;
        } else {
            // Perform a write operation
            cache_write(key, value);
            writes++;
        }

        spinlock_unlock();  // Unlocking the cache
    }

    printf("Thread %d: %d leituras, %d escritas\n", thread_id, reads, writes);
    
    // Return the read and write counts as a dynamically allocated array
    //  This array is allocated in the heap and must be freed in the main thread
    int* result = malloc(2 * sizeof(int));
    result[0] = reads;
    result[1] = writes;
    
    pthread_exit(result);
}

int main() {
    srand(time(NULL));

    // Initialize the cache and spinlock
    cache_init();
    spinlock_init();

    // Thread creation
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
    printf("Iniciando %d threads para realizar operações no cache...\n", NUM_THREADS);

    struct timespec start_time, end_time;   // Time variables 
    clock_gettime(CLOCK_MONOTONIC, &start_time); // Start time

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    // Wait for threads to finish and collect statistics
    int total_reads = 0;
    int total_writes = 0;
    int* thread_result;
    
    for (int i = 0; i < NUM_THREADS; i++) { // Wait for all threads to finish
        pthread_join(threads[i], (void**)&thread_result);
        total_reads += thread_result[0];
        total_writes += thread_result[1];
        free(thread_result);
    }

    clock_gettime(CLOCK_MONOTONIC, &end_time); // End time

    // Calculate execution time
    double execution_time = (end_time.tv_sec - start_time.tv_sec) + 
                            (end_time.tv_nsec - start_time.tv_nsec) / 1e9; // Execution time in with nanoseconds precision
    
    // Calculate total operations and operations per second
    int total_operations = total_reads + total_writes;
    double ops_per_second = total_operations / execution_time;
    
    // Print statistics
    printf("\n--- Estatísticas de Execução ---\n");
    printf("Tempo de execução: %.3f segundos\n", execution_time);
    printf("Total de leituras: %d\n", total_reads);
    printf("Total de escritas: %d\n", total_writes);
    printf("Total de operações: %d\n", total_operations);
    printf("Operações por segundo: %.2f\n", ops_per_second);

    return 0;
}
