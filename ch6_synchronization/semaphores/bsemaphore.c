//fatto a lezione
              
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<suspend.h>
#include<tlist.h>
#include<semaphore.h>

#define mutex_in(X) pthread_mutex_lock(X)
#define mutex_out(X) pthread_mutex_unlock(X)

struct semaphore {
	volatile long value;
	pthread_mutex_t lock;
	struct tlist *q;
};

semaphore semaphore_create(long initval) {
	semaphore s = malloc(sizeof(*s));
	if (s) {
		s->value = initval;
		s->q = NULL;
		pthread_mutex_init(&s->lock, NULL);
	}
	return s;
}

void semaphore_destroy(semaphore s) {
	pthread_mutex_destroy(&s->lock);
	free(s);
}

void semaphore_P(semaphore s) {
	mutex_in(&s->lock);
	if (s->value <= 0) {
		tlist_enqueue(&s->q, pthread_self());
		mutex_out(&s->lock);// IMPORTANTE. Esco poi sospendo
		suspend();
	} else {
		s->value--;
		mutex_out(&s->lock);
	}
}

void semaphore_V(semaphore s) {
	mutex_in(&s->lock);
	if (tlist_empty(s->q)) 
		s->value++;
	else
		wakeup(tlist_dequeue(&s->q));
	mutex_out(&s->lock);
}
