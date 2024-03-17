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
/**
 * [ACQUIRE]
 * [RELEASE]
 * [WAITING]
*/
void acquire_semaphore(Semaphore *sem, pid_t pid) {
    // Mae aún hay campo, éntrele sin miedo
    if(sem->count > 0){
        sem->count--;
        printf("ACQUIRE: [PID %d]\n", pid);
        enqueue(sem->queue, pid);
    }
    // Mae ya no hay campo, vaya a esperar
    else {
        printf("WAITING: [PID %d]\n", pid);
        enqueue(sem->blocked_queue, pid);
    }
}

void release_Semaphore(Semaphore *sem) {
    // Para que la cola se desocupe sólo hace falta que esté llena
    if(sem->count == 0){
        sem->count++;
        printf("RELEASE: [PID %d] -> ", dequeue(sem->queue));
        
        // Mae meta al siguiente en la vara xd
        int first_blocked = dequeue(sem->blocked_queue);
        acquire_semaphore(sem, first_blocked);
    }
}
