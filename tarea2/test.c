#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "structs/queue.c"
#include "structs/semaphore.c"

Semaphore *sem;



void testQueue(){
    Queue *queue = createQueue();

    // Ejemplo de uso
    enqueue(queue, 10);
    enqueue(queue, 20);
    enqueue(queue, 30);

    while (!isEmpty(queue)) {
        printf("%d ", dequeue(queue));
    }

    printf("\n");
    // Liberar la memoria de la cola
    free(queue); 
}

void *thread_acquire_sem(void *arg) {
    pid_t pid = *(pid_t *)arg;
    
    // Entran
    acquire_semaphore(sem, pid);

    sleep((rand() % (5 - 1 + 1)) + 1);

    // Salen
    release_Semaphore(sem);
}

void testSem(){
    int slots = 3;
    sem = createSemaphore(slots);

    // Inicializar los procesos por atender
    int attends = slots*3;
    pid_t tids[attends];
    pthread_t threads[attends];

    // Crear ids
    for (int i = 0; i < attends; i++) {
        tids[i] = i;
    }
    
    // Crear hilos
    for (int i = 0; i < attends; i++) {
        pthread_create(&threads[i], NULL, thread_acquire_sem, &tids[i]);
    }
    
    // Esperar a que terminen
    for (int i = 0; i < attends; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    testSem();
    return 0;
}
