/*
 * sm: educational resources to teach concurrent programming
 * Copyright (C) 2016  Renzo Davoli, University of Bologna
 *      
 * producer consumer
 *        
 * sm is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *             
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 *                   
 */                   
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
 
semaphore full;
semaphore empty;
volatile int buffer;
/*aspetto un tempo random con massimo 2s e genero un intero, a questo punto
semaphore_P è empty, facendo in questo modo il produttore scopre che il buffer
è vuoto e lo dichiara non più vuoto, in questo momento non è né vuoto né pieno,
mette il valore nel buffer condiviso e setta semaphore_V a full permettendo al
consumatore di leggere.*/
void *producer(void *arg) {
	while (1) {
		int value;
		usleep(random() % 2000000);
		value = random() % 32768;
		printf("produced: %d\n",value);
		semaphore_P(empty);
		buffer = value;
		semaphore_V(full);
		printf("sent: %d\n",value);
	}
}
/*il consumatore arriva a quel semaphore_P che è pieno quando può va avanti, si
copia in una variabile locale il buffer e semaphore_P diventa non pieno e non
vuoto. Stampa il valore e si mette "a dormire" per un tempo arbitrario.*/
void *consumer(void *arg) {
	while (1) {
		int value;
		printf("\t\tconsumer ready\n");
		semaphore_P(full);
		value = buffer;
		semaphore_V(empty);
		printf("\t\tconsume %d\n", value);
		usleep(random() % 2000000);
	}
}
 
int main(int argc, char *argv[]) {
	pthread_t prod_t;
	pthread_t cons_t;
	full=semaphore_create(0);
	empty=semaphore_create(1);
	srandom(time(NULL));
	pthread_create(&prod_t, NULL, producer, NULL);
	pthread_create(&cons_t, NULL, consumer, NULL);
	pthread_join(prod_t, NULL);
	pthread_join(cons_t, NULL);
}