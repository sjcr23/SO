#ifndef sync_h
#define sync_h

#include <pthread.h>


/**
 * QUEUE
*/
typedef struct Node {
    pid_t pid;
    struct Node *next;
} Node;


typedef struct {
    Node *head;
    Node *tail;
} Queue;

Queue* createQueue();
void enqueue(Queue *queue, pid_t data);
pid_t dequeue(Queue *queue);
int isEmpty(Queue *queue);


/**
 * SEMAPHORE 
*/
typedef struct {
    int count;
    Queue *queue;
    Queue *blocked_queue;
} Semaphore;

Semaphore *createSemaphore(int slots);
void acquire_semaphore(Semaphore *sem, pid_t pid);
void release_Semaphore(Semaphore *sem);


/**
 * BARRIER 
*/
typedef struct {
    int count;
    int max_count;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Barrier;

void barrier_init(Barrier *barrier, int count);
void barrier_wait(Barrier *barrier);


/**
 * MONITOR 
*/
#define BUFFER_SIZE 10 // Definimos el tamaño máximo del buffer para el monitor

// Estructura para el monitor
typedef struct {
    int buffer[BUFFER_SIZE]; // Buffer para almacenar los datos
    int count; // Contador de elementos en el buffer
    int in; // Índice de inserción en el buffer
    int out; // Índice de extracción del buffer
    pthread_mutex_t mutex; // Mutex para garantizar la exclusión mutua
    pthread_cond_t not_full; // Variable de condición para señalar cuando el buffer no está lleno
    pthread_cond_t not_empty; // Variable de condición para señalar cuando el buffer no está vacío
} Monitor;

// Funciones para el monitor
void monitor_init(Monitor *mon); // Inicializa el monitor
void produce(Monitor *mon, int data); // Función para que el productor produzca datos y los inserte en el buffer
int consume(Monitor *mon); // Función para que el consumidor extraiga datos del buffer

#endif /* sync_h */
