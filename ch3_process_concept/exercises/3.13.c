#include <stdio.h>
#include <stdlib.h>

#define MIN_PID 300
#define MAX_PID 5000

// allocate_map
int pid[MAX_PID - MIN_PID] = {0};

int allocate_pid(void)
{
    int i, flag = 1;
    for (i = 0; i < MAX_PID - MIN_PID; i++)
    {
        if (pid[i] == 0)
        {
            pid[i] = 1;
            flag = 0;
            break;
        }
    }
    return flag ? 1 : i;
}

void release_pid(int id)
{
    pid[id] = 0;
}