#include <pthread.h>
#include <stdio.h>

#include "structs/sync.h"

#define DATA_SIZE 20

// Función de hilo para el productor
void *producer_thread(void *arg) {
    Monitor *mon = (Monitor *)arg;
    // Produce datos en el monitor
    for (int i = 0; i < DATA_SIZE; i++) {
        produce(mon, i);
        // Imprime el dato producido
        printf("Produced: %d\n", i);
    }
    pthread_exit(NULL);
}

// Función de hilo para el consumidor
void *consumer_thread(void *arg) {
    Monitor *mon = (Monitor *)arg;
    // Consume datos del monitor
    for (int i = 0; i < DATA_SIZE; i++) {
        int data = consume(mon);
        // Imprime el dato consumido
        printf("Consumed: %d\n", data);
    }
    pthread_exit(NULL);
}

// Función principal
int main() {
    pthread_t producer, consumer;
    Monitor monitor;
    monitor_init(&monitor); // Inicializa el monitor

    // Crear hilos para productor y consumidor
    pthread_create(&producer, NULL, producer_thread, &monitor);
    pthread_create(&consumer, NULL, consumer_thread, &monitor);

    // Esperar a que los hilos terminen
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);

    return 0;
}
