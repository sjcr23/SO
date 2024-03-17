#include <pthread.h>
#include <stdio.h>

#include "sync.h" // Incluimos el archivo de encabezado que contiene las definiciones del monitor

// Inicializar el monitor
void monitor_init(Monitor *mon) {
    // Inicializamos tanto el contador de elementos, como los índices del buffer, a cero
    mon->count = 0;
    mon->in = 0; 
    mon->out = 0;
    // Inicializamos el mutex del monitor
    pthread_mutex_init(&mon->mutex, NULL); 
     // Inicializamos la variable de condición para el caso de buffer no lleno
    pthread_cond_init(&mon->not_full, NULL);
    // Inicializamos la variable de condición para el caso de buffer no vacío
    pthread_cond_init(&mon->not_empty, NULL); 
}

// Productor
void produce(Monitor *mon, int data) {
    pthread_mutex_lock(&mon->mutex);
    // Mientras el buffer esté lleno, esperamos
    while (mon->count == BUFFER_SIZE) { 
        // El wait está adentro del while con la condición deseada para evitar falsos positivos
        // El hilo actual se manda a esperar/dormir hasta que la condición se cumpla
        // La condición se activa cuando otro hilo en el consumidor realiza el "signal"
        pthread_cond_wait(&mon->not_full, &mon->mutex);
    }
    // Insertamos el dato en el buffer
    mon->buffer[mon->in] = data; 
    // Actualizamos el índice de inserción
    mon->in = (mon->in + 1) % BUFFER_SIZE; 
    // Incremento del contador de elementos
    mon->count++;
    // Señalizamos que el buffer no está vacío
    // Esto despierta a los hilos en espera en el consumidor
    pthread_cond_signal(&mon->not_empty); 
    pthread_mutex_unlock(&mon->mutex);
}

// Consumidor
int consume(Monitor *mon) {
     // Variable para almacenar el dato a consumir
    int data;
    pthread_mutex_lock(&mon->mutex);
    // Mientras el buffer esté vacío, esperamos
    while (mon->count == 0) { 
        // El wait está adentro del while con la condición deseada para evitar falsos positivos
        // El hilo actual se manda a esperar/dormir hasta que la condición se cumpla
        // La condición se activa cuando otro hilo en el productor realiza el "signal"
        pthread_cond_wait(&mon->not_empty, &mon->mutex);
    }
    // Extraemos el dato del buffer
    data = mon->buffer[mon->out]; 
    // Actualizamos el índice de extracción
    mon->out = (mon->out + 1) % BUFFER_SIZE; 
    // Disminuimos el contador de elementos en el buffer
    mon->count--; 
    // Señalizamos que el buffer no está lleno
    // Esto reactiva/despierta a los hilos en el productor
    pthread_cond_signal(&mon->not_full); 
    pthread_mutex_unlock(&mon->mutex);
    return data;
}


