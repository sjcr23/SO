#include <stdio.h>
#include <stdlib.h>
#include "structs/queue.c"
#include "structs/semaphore.c"
#include "structs/barrier.c"
#include "structs/monitor.c"


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



void testSem(){
    // Tamaño del semáforo
    int slots = 3;
    Semaphore *sem = createSemaphore(slots);

    // Procesos por atender
    int attends = slots*2;
    int pids[attends];
    for (int i = 0; i < attends; i++){
        pids[i] = i;
        acquire_semaphore(sem, pids[i]);   // Acá piden ser atendidos
    }
    
    // 3 out
    printf("\n");
    release_Semaphore(sem);
    release_Semaphore(sem);
    release_Semaphore(sem);
    free(sem);
}

int main() {
    testSem();
    return 0;
}
