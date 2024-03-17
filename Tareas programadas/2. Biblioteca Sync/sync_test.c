#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "structs/queue.c"
#include "structs/semaphore.c"
#include "structs/barrier.c"
#include "structs/monitor.c"

#define DATA_SIZE 30

Queue *queue;
Semaphore *sem;
Barrier *bar;
Monitor *mon;



/**
 * QUEUE
*/
void testQueue(){
    printf("==================== TEST QUEUE ====================\n");

    queue = createQueue();

    // Ejemplo de uso
    printf("Adding: 10, 20, 30\n");
    enqueue(queue, 10);
    enqueue(queue, 20);
    enqueue(queue, 30);

    printf("Queue: ");
    while (!isEmpty(queue)) {
        printf("%d ", dequeue(queue));
    }

    printf("\n");
    // Liberar la memoria de la cola
    free(queue); 
}

    
/**
 * SEMAPHORE
*/
void *acquire_sem(void *arg) {
    pid_t pid = *(pid_t *)arg;
    acquire_semaphore(sem, pid);
    sleep((rand() % (3 - 1 + 1)) + 1);
    release_semaphore(sem);
    pthread_exit(NULL);
}

void testSem(){
    printf("================== TEST SEMAPHORE ==================\n");
    
    int slots = 3;
    sem = create_semaphore(slots);

    // Inicializar los procesos por atender
    int attends = slots*3;
    pid_t tids[attends];
    pthread_t threads[attends];
   
    // Crear hilos e IDs
    for (int i = 0; i < attends; i++) {
        tids[i] = i;
        pthread_create(&threads[i], NULL, acquire_sem, &tids[i]);
    }
    // Esperar a que terminen
    for (int i = 0; i < attends; i++) {
        pthread_join(threads[i], NULL);
    }
    free(sem);
}

/**
 * BARRIER
*/
void *acquire_barr(void *arg) {
    pid_t pid = *(pid_t *)arg;
    barrier_wait(bar, pid);
    pthread_exit(NULL);
}

void testBarrier(){
    printf("=================== TEST BARRIER ===================\n");

    int NUM_THREADS = 5;
    bar = create_barrier(NUM_THREADS);

    int attends = NUM_THREADS*2;
    int tids[attends];
    pthread_t threads[attends];
    
    // Crear hilos e IDs
    for (int i = 0; i < attends; i++) {
        tids[i] = i;
        pthread_create(&threads[i], NULL, acquire_barr, &tids[i]);
    }
    // Esperar a que terminen
    for (int i = 0; i < attends; i++) {
        pthread_join(threads[i], NULL);
    }
    free(bar);
}


/**
 * MONITOR
*/
// Función de hilo para el productor
void *producer_thread(void *arg) {
    // Produce datos en el monitor
    for (int i = 0; i < DATA_SIZE; i++) {
        produce(mon, i);
        // Imprime el dato producido
        printf("• [PRODUCED %d]\n", i);
    }
    pthread_exit(NULL);
}

// Función de hilo para el consumidor
void *consumer_thread(void *arg) {
    // Consume datos del monitor
    for (int i = 0; i < DATA_SIZE; i++) {
        int data = consume(mon);
        // Imprime el dato consumido
        printf("- [CONSUMED %d]\n", data);
    }
    pthread_exit(NULL);
}

void testMonitor(){
    printf("=================== TEST MONITOR ===================\n");

    pthread_t producer, consumer;
    mon = create_monitor();

    // Crear hilos para productor y consumidor
    pthread_create(&producer, NULL, producer_thread, NULL);
    pthread_create(&consumer, NULL, consumer_thread, NULL);

    // Esperar a que los hilos terminen
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
}

int main() {
    testQueue();
    testSem();
    testBarrier();
    testMonitor();
    return 0;
}
