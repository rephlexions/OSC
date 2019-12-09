#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

int average, min, max;

void *averageRunner(void *param); /* the thread */

/* void *minRunner(void *param);

void *maxRunner(void *param); 
 */

int main(int argc, char *argv[])
{
    pthread_t tid1, tid2, tid3;
    pthread_attr_t attr1, attr2, attr3;

    if (argc <= 1)
    {
        fprintf(stderr, "usage: a.out <integer value> <integer value> <integer value> ...\n");
        /*exit(1);*/
        return -1;
    }

    /* get the default attributes */
    pthread_attr_init(&attr1);

    /* create the thread */
    pthread_create(&tid1, &attr1, averageRunner, argv);

    /* now wait for the thread to exit */
    pthread_join(tid1, NULL);

    printf("Average = %d\n", average);
}

void *averageRunner(void *param) /* the thread */
{
    char **argv = param;
    int argc = 1;
    int sum = 0;

    while (argv[argc] != NULL)
    {
        printf("argv[%d] = %s\n", argc, argv[argc]);
        argc++;
        // FIXME: Segmentation fault
        //sum += atoi(argv[argc]);
        //int n = (int)strtol(argv[argc], (char **)NULL, 10);
    }
    printf("Sum: %d \n", sum);
    pthread_exit(0);
}

/* void *minRunner(void *param);
{
}
void *maxRunner(void *param); 
{
} */