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

Semaphore *create_semaphore(int slots);
void acquire_semaphore(Semaphore *sem, pid_t pid);
void release_semaphore(Semaphore *sem);



/**
 * BARRIER 
*/
typedef struct {
    int count;
    int max_count;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Barrier;

Barrier *create_barrier(int count);
void barrier_wait(Barrier *barrier, pid_t pid);



/**
 * MONITOR 
*/
#define BUFFER_SIZE 10 // Definimos el tamaño máximo del buffer para el monitor

// Estructura para el monitor
typedef struct {
    int buffer[BUFFER_SIZE];
    int count;
    int in;
    int out;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} Monitor;

// Funciones para el monitor
Monitor *create_monitor();
void produce(Monitor *mon, int data);
int consume(Monitor *mon);

#endif /* sync_h */
