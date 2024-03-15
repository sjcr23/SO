#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sync.h"

// Función para inicializar un semáforo
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

void acquire_semaphore(Semaphore *sem, int pid) {
    // Mae aún hay campo, éntrele sin miedo
    if(sem->count > 0){
        sem->count--;
        printf("PID: %d acquired Sem.\n", pid);
        enqueue(sem->queue, pid);
    }
    // Mae ya no hay campo, vaya a esperar
    else {
        printf("PID: %d is now waiting. (Sem is full)\n", pid);
        enqueue(sem->blocked_queue, pid);
    }
}

void release_Semaphore(Semaphore *sem) {
    // Para morir sólo hace falta estar vivo
    // (para que la cola se desocupe sólo hace falta que esté llena)
    if(sem->count == 0){
        sem->count++;
        printf("PID: %d released Sem.  ->  ", dequeue(sem->queue));
        
        // Mae meta al siguiente en la vara xd
        int first_blocked = dequeue(sem->blocked_queue);
        acquire_semaphore(sem, first_blocked);
    }
}
