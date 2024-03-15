#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "structs/barrier.c"

#define NUM_THREADS 5

Barrier barrier;


void *thread_function(void *arg) {
    int thread_id = *(int *)arg;


    printf("tid %d is waiting at the barrier.\n", thread_id);
    barrier_wait(&barrier);
    printf("tid %d has passed the barrier.\n", thread_id);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    barrier_init(&barrier, NUM_THREADS);

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, (void *)&thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}