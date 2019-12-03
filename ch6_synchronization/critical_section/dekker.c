/* 
Compile with:
gcc dekker.c -o dekker -lpthread


uintptr_t
unsigned int that is capable of storing a pointer

__sync_synchronize()
these builtins are considered a full barrier. That is, no memory operand will be moved across the operation,
either forward or backward. Further, instructions will be issued as necessary to prevent the processor from
speculating loads across the operation and from queuing stores after the operation. 

sched_yeld()
sched_yield() causes the calling thread to relinquish the CPU. The thread is moved to the end of the queue
 for its static priority and a new thread gets to run. 
*/
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<sched.h>
#include<pthread.h>
#include<unistd.h>
 
#define MAX 200
#define STEP 10
#define NPROC 2
 
volatile long val = 0;
int need[NPROC];
int turn;
 
void *codet(void *arg) {
  uintptr_t id = (uintptr_t) arg;
  int count;
  long tmp;
  for (count=0; count < MAX; count++) {
    /* enter */
    need[id] = 1;
    // https://gcc.gnu.org/onlinedocs/gcc-4.6.2/gcc/Atomic-Builtins.html
    __sync_synchronize();
    while (need[1 - id]) {
     __sync_synchronize();
      if (turn != id) {
        need[id] = 0;
        while (turn != id)
        // http://man7.org/linux/man-pages/man2/sched_yield.2.html
          sched_yield();
        need[id] = 1;
      }
      __sync_synchronize();
    }
    /* /enter */
    tmp = val;
    //usleep(rand() % 2);
    val = tmp + 1;
    if (count % STEP == 0)
      printf("%zu %ld\n",id,val);
    /* exit */
    turn = 1 - id;
    need[id] = 0;
    /* /exit */
  }
}
 
int main(int argc, char *argv[]) {
  
  uintptr_t i;
  // pthread_t used to identify a thread.
  pthread_t t[NPROC];
  srand(time(NULL));
  
  for (i=0; i<NPROC; i++)
    // https://stackoverflow.com/questions/6990888/c-how-to-create-thread-using-pthread-create-function
    pthread_create(&t[i], NULL, codet, (void *) i);
  
  for (i=0; i<NPROC; i++)
  // http://man7.org/linux/man-pages/man3/pthread_join.3.html
    pthread_join(t[i], NULL);

  printf("%ld %d\n",val,NPROC*MAX);
}