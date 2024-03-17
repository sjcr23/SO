#include <stdio.h>
#include <time.h>

int fibo(int n){
  if(n<=1) return 1;
  //printf(".");
  return fibo(n-1)+fibo(n-2);
}

int main() {
  int n = 0;
  scanf("%d", &n);
  time_t then = time(0);
  fibo(n);
  time_t now = time(0);
  // printf("fibo(%d) = %d \n",n, fibo(n));
  printf("El tiempo es %d \n", difftime(now, then));
  return 0;
}
