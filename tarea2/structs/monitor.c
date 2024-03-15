#include <pthread.h>

#include "sync.h"


void monitor_init(Monitor *monitor) {
    pthread_mutex_init(&monitor->mutex, NULL);
    pthread_cond_init(&monitor->cond, NULL);
    monitor->count = 0;
}

// Procedimiento del Monitor para incrementar el contador
void increment(Monitor *monitor) {
    pthread_mutex_lock(&monitor->mutex);
    monitor->count++;
    printf("Incrementado: %d\n", monitor->count);
    pthread_cond_signal(&monitor->cond);
    pthread_mutex_unlock(&monitor->mutex);
}

// Procedimiento del Monitor para reducir el contador
void decrement(Monitor *monitor) {
    pthread_mutex_lock(&monitor->mutex);
    while (monitor->count == 0) {
        pthread_cond_wait(&monitor->cond, &monitor->mutex);
    }
    monitor->count--;
    printf("Derecementado: %d\n", monitor->count);
    pthread_mutex_unlock(&monitor->mutex);
}



/* void monitor_enter(Monitor *monitor) {
    pthread_mutex_lock(&monitor->mutex);
}

void monitor_exit(Monitor *monitor) {
    pthread_mutex_unlock(&monitor->mutex);
}

void monitor_wait(Monitor *monitor) {
    pthread_cond_wait(&monitor->cond, &monitor->mutex);
}

void monitor_notify(Monitor *monitor) {
    pthread_cond_signal(&monitor->cond);
} */


