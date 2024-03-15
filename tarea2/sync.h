#ifndef sync_h
#define sync_h

#include <pthread.h>



typedef struct {
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Semaphore;

void semaphore_init(Semaphore *sem, int initial_count);
void semaphore_wait(Semaphore *sem);
void semaphore_signal(Semaphore *sem);




typedef struct {
    int count;
    int max_count;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Barrier;

void barrier_init(Barrier *barrier, int count);
void barrier_wait(Barrier *barrier);



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
