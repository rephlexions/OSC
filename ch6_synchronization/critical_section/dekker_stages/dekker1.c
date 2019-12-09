#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX 100
#define turnP 0
#define turnQ 1

_Atomic volatile int turn = 0;

void *P(void *v)
{
	int i;
	for (i = 0; i < MAX; i++)
	{
		while (turn == turnQ)
			;
		printf("P IN\n");
		usleep(random() % 200000 + 1000000);
		printf("P OUT\n");
		turn = turnQ;
	}
}

void *Q(void *v)
{
	int i;
	for (i = 0; i < MAX; i++)
	{
		while (turn == turnP)
			;
		printf("Q IN\n");
		usleep(random() % 200000);
		printf("Q OUT\n");
		turn = turnP;
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
