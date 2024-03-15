#include <pthread.h>
#include "sync.h"



void barrier_init(Barrier *barrier, int count) {
    barrier->count = 0;
    barrier->max_count = count;
    pthread_mutex_init(&barrier->mutex, NULL);
    pthread_cond_init(&barrier->cond, NULL);
}

void barrier_wait(Barrier *barrier) {
    pthread_mutex_lock(&barrier->mutex);
    barrier->count++;
    if (barrier->count >= barrier->max_count) {
        barrier->count = 0;
        pthread_cond_broadcast(&barrier->cond);
    } else {
        while (barrier->count < barrier->max_count) {
            pthread_cond_wait(&barrier->cond, &barrier->mutex);
        }
    }
    pthread_mutex_unlock(&barrier->mutex);
}
