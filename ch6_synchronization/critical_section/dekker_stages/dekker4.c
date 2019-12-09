#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX 100

_Atomic volatile int inp;
_Atomic volatile int inq;

void *P(void *v)
{
	int i;
	for (i = 0; i < MAX; i++)
	{
		inp = 1;
		while (inq)
		{
			inp = 0;
			usleep(random() % 1000);
			inp = 1;
		}
		printf("P IN\n");
		usleep(random() % 200000);
		printf("P OUT\n");
		inp = 0;
	}
}

void *Q(void *v)
{
	int i;
	for (i = 0; i < MAX; i++)
	{
		inq = 1;
		while (inp)
		{
			inq = 0;
			usleep(random() % 1000);
			inq = 1;
		}
		printf("Q IN\n");
		usleep(random() % 200000);
		printf("Q OUT\n");
		inq = 0;
	}
}

int main(int argc, char *argv[])
{
	srandom(time(NULL));
	pthread_t pp;
	pthread_t qq;
	pthread_create(&pp, NULL, P, NULL);
	pthread_create(&qq, NULL, Q, NULL);
	pthread_join(pp, NULL);
	pthread_join(qq, NULL);
}
