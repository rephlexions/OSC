#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void *runner(void *param);

int main(int argc, char *argv[])
{
    pthread_t tid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, runner, argv[1]);
    pthread_join(tid, NULL);
}

// FIXME: no output
void *runner(void *param)
{
    int n = atoi(param);
    int fact, i, j;
    printf("Prime Numbers are: \n");
    for (i = 2; i <= i / 2; i++)
    {
        fact = 1;

        for (j = 2; j <= n; j++)
        {
            if (i % j == 0)
            {
                fact = 0;
                break;
            }
        }
        if (fact == 1)
        {
            printf("%d ", i);
        }
        pthread_exit(0);
    }
}