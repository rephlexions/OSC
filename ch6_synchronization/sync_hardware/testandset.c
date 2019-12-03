#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>

#define MAX 4000000
#define STEP 1000000
#define NPROC 4

volatile long val = 0;
volatile long lock = 0;

void *codet(void *arg)
{
  uintptr_t id = (uintptr_t)arg;
  int count;
  for (count = 0; count < MAX; count++)
  {
    /* enter */
    while (__sync_lock_test_and_set(&lock, 1))
      /*sched_yield()*/;
    /* /enter */
    val = val + 1;
    if (count % STEP == 0)
      printf("%zu %ld\n", id, val);
    /* exit */
    __sync_lock_release(&lock);
    /* /exit */
  }
}

int main(int argc, char *argv[])
{
  uintptr_t i;
  pthread_t t[NPROC];
  for (i = 0; i < NPROC; i++)
    pthread_create(&t[i], NULL, codet, (void *)i);
  for (i = 0; i < NPROC; i++)
    pthread_join(t[i], NULL);
  printf("%zu %d\n", val, NPROC * MAX);
}