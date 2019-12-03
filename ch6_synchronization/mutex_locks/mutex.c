#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
 
#define NUM_THREADS     5
#define MAX 1000000
 
int sum;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
 
void* perform_work(){
	int i;
 
	for(i = 0; i < MAX; i++){ 
		pthread_mutex_lock(&mutex);		
		sum = sum + 1;
		pthread_mutex_unlock(&mutex);
	}
  	/* optionally: insert more useful stuff here */ 
  	return NULL;
}
  
int main( int argc, char** argv ){

	pthread_t threads[NUM_THREADS];
  	int thread_args[NUM_THREADS];
  	int result_code;
  	unsigned index;
 
	sum = 0;
  	// create all threads one by one
  	for(index = 0; index < NUM_THREADS; ++index ){
    		thread_args[index] = index;
    		printf("In main: creating thread %d\n", index);
    		result_code = pthread_create(&threads[index], NULL, perform_work, NULL);
			printf("Result Code: %d\n", result_code);
    		assert(!result_code);
  	}
	  printf("\n"); 
  	// wait for each thread to complete
  	for(index = 0; index < NUM_THREADS; ++index ){
    		// block until thread 'index' completes
    		result_code = pthread_join(threads[index], NULL);
    		assert(!result_code);
    		printf("In main: thread %d has completed\n", index);
			printf("Result Code: %d\n", result_code);
   	}
 
   	printf("In main: All threads completed successfully\n" );
	printf("Result: %d\n", sum);
   	exit( EXIT_SUCCESS );
}