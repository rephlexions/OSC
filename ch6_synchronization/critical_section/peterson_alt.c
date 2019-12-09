#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define N 5

volatile int stage[N];
volatile int last[N];

void *proc(void *arg)
{
    uintptr_t i = (uintptr_t)arg;
    int j, k;

    for (;;)
    {
        for (int j = 0; j < N; j++)
        {
            stage[i] = j;
            last[j] = i;

            for (k = 0; k < N; k++)
            {
                if (i != k)
                    while (stage[k] >= stage[i] && last[j] == i)
                        ;
            }
        }
    }

    printf("%ldin ", i);
    fflush(stdout);
    usleep((rand() % 100000) * (rand() % 10));

    printf("%ldout\n", i);
    stage[i] = 0;
    usleep((rand() % 100000) * (rand() % 10));
}

int main(int argc, char *argv[])
{
    uintptr_t i;
    srandom(time(NULL));
    pthread_t t[N];

    for (i = 0; i < N; i++)
        pthread_create(t + i, NULL, proc, (void *)i);

    pause();
    return 0;
}