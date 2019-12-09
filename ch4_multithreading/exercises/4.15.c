#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

#define MIN_PID 100
#define MAX_PID 1000
#define NO_OF_P 16 //no of process/threads

//not using map (bitmap), using a preallocated char array instead
int pid[MAX_PID - MIN_PID] = {0};

void *threadRoutine(void *arg);

int allocate_pid(void);

void release_pid(int id);

int main()
{
    int i;
    pthread_t process[NO_OF_P];

    srand(time(NULL));

    for (i = 0; i < NO_OF_P; i++)
    {
        if (pthread_create(&process[i], NULL, threadRoutine, (void *)&i))
            return -1 * printf("Error in thread %d creation!!!\n", i); ///return a negative integer
    }

    for (i = 0; i < NO_OF_P; i++)
    {
        pthread_join(process[i], NULL);
    }

    //wait(NULL);
    return 0 * printf("\nSUCCESSFUL EXIT\n");
}

void *threadRoutine(void *arg)
{
    int tid = *((int *)arg);

    //get a pid
    int id = allocate_pid();

    if (id == -1)
    {
        puts("No PID available.");
    }
    else
    {
        printf("Thread [%3d] PID [%3d] Allocated\n", tid, id + MIN_PID);
        //sleep for a random time between 1-10 seconds

        int r = 1 + rand() % 30;
        //uncomment next line to make the thread sleep for a random time
        //sleep(r);

        printf("Thread [%3d] PID [%3d] Released after %d sec\n", tid, id + MIN_PID, r);
        release_pid(id);
    }
    pthread_exit(NULL);
}

int allocate_pid(void)
{
    int i, flag = 1;
    for (i = 0; i < MAX_PID - MIN_PID; i++)
    {
        if (pid[i] == 0)
        {
            //pid available//next line should be in critical section
            //with a mutex lock, whicever thread gets the lock gets the id.
            pid[i] = 1;
            flag = 0;
            break;
        }
    }
    return flag ? -1 : i;
}
void release_pid(int id)
{
    pid[id] = 0;
}
