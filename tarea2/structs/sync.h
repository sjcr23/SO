#ifndef sync_h
#define sync_h

#include <pthread.h>


/**
 * QUEUE
*/
typedef struct Node {
    int pid;
    struct Node *next;
} Node;


typedef struct {
    Node *head;
    Node *tail;
} Queue;

Queue* createQueue();
void enqueue(Queue *queue, int data);
int dequeue(Queue *queue);
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
void acquire_semaphore(Semaphore *sem, int pid);
void semaphore_signal(Semaphore *sem);


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
typedef struct {
    int data;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Monitor;

void monitor_init(Monitor *monitor);
void monitor_enter(Monitor *monitor);
void monitor_exit(Monitor *monitor);
void monitor_wait(Monitor *monitor);
void monitor_notify(Monitor *monitor);

#endif /* sync_h */
