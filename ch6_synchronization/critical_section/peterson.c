/*
Compile with:
gcc peterson.c -o peterson -lpthread

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
  //uintptr_t an unsigned int that is capable of storing a pointer
  uintptr_t id = (uintptr_t)arg;
  int count;
  for (count = 0; count < MAX; count++)
  {
    /* enter */
    need[id] = 1;
    turn = 1 - id;
    // https://gcc.gnu.org/onlinedocs/gcc-4.6.2/gcc/Atomic-Builtins.html
    __sync_synchronize();
    while (need[1 - id] && turn != id)
      ;
    /* /enter */
    val = val + 1;
    if (count % STEP == 0)
      printf("%zu %ld\n", id, val);
    /* exit */
    need[id] = 0;
    /* /exit */
  }
}

int main(int argc, char *argv[])
{
  uintptr_t i;
  pthread_t t[NPROC];

  for (i = 0; i < NPROC; i++){
    // https://stackoverflow.com/questions/6990888/c-how-to-create-thread-using-pthread-create-function
    pthread_create(&t[i], NULL, codet, (void *)i);
  }

  for (i = 0; i < NPROC; i++){
    // http://man7.org/linux/man-pages/man3/pthread_join.3.html
    pthread_join(t[i], NULL);
  }

  printf("%zu %d\n", val, NPROC * MAX);
}