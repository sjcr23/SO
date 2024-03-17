#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sync.h"

// Crear un semáforo en memoria
Semaphore *createSemaphore(int slots) {
    Semaphore *sem = calloc(1, sizeof(Semaphore));
    if (sem == NULL) {
        printf("Error: No se pudo asignar memoria para el semáforo.\n");
        exit(EXIT_FAILURE);
    }
    sem->count = slots;
    sem->queue = createQueue();
    sem->blocked_queue = createQueue();
    return sem;
} 

// Solicitar al semáforo acceso a recursos
void acquire_semaphore(Semaphore *sem, pid_t pid) {
    // Permitir el acceso si queda campo en la cola
    if(sem->count > 0){
        sem->count--;
        printf("ACQUIRE: [PID %d]\n", pid);
        enqueue(sem->queue, pid);
    }
    // Denegar el acceso si no queda campo
    else {
        printf("WAITING: [PID %d]\n", pid);
        enqueue(sem->blocked_queue, pid);
    }
}

void release_Semaphore(Semaphore *sem) {
    // Permitir acceso cuando la cola esté libre
    if(sem->count == 0){
        sem->count++;
        printf("RELEASE: [PID %d] -> ", dequeue(sem->queue));
        
        pid_t first_blocked = dequeue(sem->blocked_queue);
        acquire_semaphore(sem, first_blocked);
    }
}
