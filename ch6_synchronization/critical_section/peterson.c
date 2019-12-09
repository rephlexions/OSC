/*
https://gcc.gnu.org/onlinedocs/gcc-4.6.2/gcc/Atomic-Builtins.html
*/

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 4000000
#define STEP 1000000
#define NPROC 2

volatile long val = 0;
volatile long need[NPROC];
volatile long turn;

void *codet(void *arg)
{
  uintptr_t id = (uintptr_t)arg;
  int count;
  for (count = 0; count < MAX; count++)
  {
    /* enter */
    need[id] = 1;
    turn = 1 - id;
    __sync_synchronize();
    while (need[1 - id] && turn != id)
      ;

    /* critical section */
    val = val + 1;
    if (count % STEP == 0)
      printf("ID: %zu -- VAL: %ld\n", id, val);
    /* exit */
    need[id] = 0;
    /* /exit */
  }
}

int main(int argc, char *argv[])
{
  uintptr_t i;
  pthread_t t[NPROC];

  for (i = 0; i < NPROC; i++)
  {
    pthread_create(&t[i], NULL, codet, (void *)i);
  }

  for (i = 0; i < NPROC; i++)
  {
    pthread_join(t[i], NULL);
  }

  printf("VAL: %zu -- NPROC*MAX: %d\n", val, NPROC * MAX);
}