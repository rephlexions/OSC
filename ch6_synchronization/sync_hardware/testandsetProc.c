//test and set fra processi
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/mman.h>

#define MAX 400000000
#define STEP 1000000

volatile int *shared;
#define val shared[0]
#define lock shared[1]

void *create_shmem(char *name, off_t length)
{
  int shmfd = shm_open(name, O_CREAT | O_RDWR, 0600);
  ftruncate(shmfd, length);
  return mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
}

int destroy_shmem(char *name)
{
  return shm_unlink(name);
}

int main(int argc, char *argv[])
{
  shared = create_shmem("test", 3 * sizeof(int));
  int count;
  for (count = 0; count < MAX; count++)
  {
    /* enter */
    while (__sync_lock_test_and_set(&lock, 1))
      sched_yield();
    /* /enter */
    val = val + 1;
    if (count % STEP == 0)
      printf("%ld\n", val);
    /* exit */
    __sync_lock_release(&lock);
    /* /exit */
  }
  printf("%d\n", val);

  destroy_shmem("test");
}