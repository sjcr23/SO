#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sync.h"

/*
   SEMÁFORO
*/
void semaphore_init(Semaphore *sem, int initial_count) {
    sem->count = initial_count;
    pthread_mutex_init(&sem->mutex, NULL);
    pthread_cond_init(&sem->cond, NULL);
}

void semaphore_wait(Semaphore *sem) {
    if (sem->count == 0 ){
        pthread_mutex_lock(&sem->mutex);
        pthread_cond_wait(&sem->cond, &sem->mutex);
    }

    sem->count--;
    pthread_mutex_unlock(&sem->mutex);
}

void semaphore_signal(Semaphore *sem) {
    pthread_mutex_lock(&sem->mutex);
    sem->count++;
    pthread_cond_signal(&sem->cond);
    pthread_mutex_unlock(&sem->mutex);
}