#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


void *whoami(void *par){
    pid_t currentPID = getpid();
    printf("Current PID is %d\n", currentPID);
    return NULL;
}

int main() {
  pid_t myPID = getpid();

  pthread_t t1;
  pthread_t t2;

  pthread_create(&t1, NULL, whoami, NULL);
  pthread_create(&t2, NULL, whoami, NULL);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  return 0;
}