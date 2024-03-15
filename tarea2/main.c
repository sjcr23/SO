#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "semaphore.c"
#include "barrier.c"
#include "monitor.c"

#define NUM_THREADS 5

Semaphore sem;
Barrier barrier;
Monitor monitor;


void *thread_function(void *arg) {
    int thread_id = *(int *)arg;

    printf("tid %ld is entering the semaphore.\n", thread_id);
    semaphore_wait(&sem);
    printf("tid %d has acquired the semaphore.\n", thread_id);
    
    // Do some work with semaphore
    semaphore_signal(&sem);
    printf("tid %d is releasing the semaphore.\n", thread_id);

    printf("tid %d is waiting at the barrier.\n", thread_id);
    barrier_wait(&barrier);
    printf("tid %d has passed the barrier.\n", thread_id);


    monitor_enter(&monitor);
    printf("tid %d is in the monitor.\n", thread_id);
    monitor_wait(&monitor);
    printf("tid %d is leaving the monitor.\n", thread_id);
    monitor_exit(&monitor);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    semaphore_init(&sem, 1);
    barrier_init(&barrier, NUM_THREADS);
    monitor_init(&monitor);
    
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, (void *)&thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}