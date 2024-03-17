#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sync.h"

// Crear un semáforo en memoria
Semaphore *create_semaphore(int slots) {
    Semaphore *sem = calloc(1, sizeof(Semaphore));
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

void release_semaphore(Semaphore *sem) {
    // Permitir acceso cuando la cola esté libre
    if(sem->count == 0){
        sem->count++;
        printf("RELEASE: [PID %d] -> ", dequeue(sem->queue));
    
        if(!isEmpty(sem->blocked_queue)){
            pid_t first_blocked = dequeue(sem->blocked_queue);
            acquire_semaphore(sem, first_blocked);
        }
        else {
            printf("La cola está vacía\n");
        }
    }
}
