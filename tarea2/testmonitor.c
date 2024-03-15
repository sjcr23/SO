#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "structs/sync.h"

#define NUM_THREADS 5


// Funciṕn para el hilo productor
void *productor(void *arg) {
    Monitor *monitor = (Monitor *)arg;
    while (1) {
        increment(monitor);
    }
    return NULL;
}

// Función para el hilo consumidor
void *consumidor(void *arg) {
    Monitor *monitor = (Monitor *)arg;
    while (1) {
        decrement(monitor);
    }
}

int main() {
    pthread_t prod_tid, cons_tid;
    Monitor monitor;

    // Inicializar el monitor
    monitor_init(&monitor);

    // Crear hilos productor y consumidor
    pthread_create(&prod_tid, NULL, productor, &monitor);
    pthread_create(&cons_tid, NULL, consumidor, &monitor);

    // Esperar a que los hilos terminen (esto nunca sucederá)
    pthread_join(prod_tid, NULL);
    pthread_join(cons_tid, NULL);

    return 0;
}