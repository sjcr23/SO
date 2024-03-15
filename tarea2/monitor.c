#include <pthread.h>

#include "sync.h"

/*
   Function to initialize the monitor structure
   It receives a Monitor struct
*/
void monitor_init(Monitor *monitor) {
    monitor->data = 0;
    // begin the mutex with the mutex contain witin the monitor struct
    pthread_mutex_init(&monitor->mutex, NULL);
    // give the condition to the monitor
    pthread_cond_init(&monitor->cond, NULL);
}

/*
    Function to enter the Monitor. It locks the mutex.
*/
void monitor_enter(Monitor *monitor) {
    pthread_mutex_lock(&monitor->mutex);
}

void monitor_exit(Monitor *monitor) {
    pthread_mutex_unlock(&monitor->mutex);
}

void monitor_wait(Monitor *monitor) {
    pthread_cond_wait(&monitor->cond, &monitor->mutex);
}

void monitor_notify(Monitor *monitor) {
    pthread_cond_signal(&monitor->cond);
}


