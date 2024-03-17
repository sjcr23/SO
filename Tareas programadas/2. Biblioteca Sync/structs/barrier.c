#include <pthread.h>
#include <stdio.h>
#include "sync.h"

Barrier *create_barrier(int size) {
    Barrier *barrier = calloc(1, sizeof(Barrier));
    barrier->count = 0;
    barrier->max_count = size;
    pthread_mutex_init(&barrier->mutex, NULL);
    pthread_cond_init(&barrier->cond, NULL);
    return barrier;
}

void barrier_wait(Barrier *bar, pid_t pid) {

    // Siempre admite
    pthread_mutex_lock(&bar->mutex);
    sleep(1);
    bar->count++;
    printf("ACQUIRE: [PID %d][%d/%d]\n", pid, bar->count, bar->max_count);

    // Cuando está lleno avisa
    if (bar->count >= bar->max_count) {
        printf("==== BARRIER IS FULL =====\n");
        pthread_cond_broadcast(&bar->cond);
    }
    // Manda a esperar cuando supera el límite
    else if (bar->count < bar->max_count && bar->count > 0) {
        pthread_cond_wait(&bar->cond, &bar->mutex);
    }
    // Los suelta mientras haya
    if(bar->count > 0){
        sleep(1);
        bar->count--;
        printf("RELEASE: [PID %d][%d/%d]\n", pid, bar->count, bar->max_count);
    }
    pthread_mutex_unlock(&bar->mutex);
}
