#include <stdio.h> 
#include <pthread.h> 
#include <stdint.h>

void *worker(void *VoidPtr) {
	/**converts void pointer to a value of int */
	int arg = (int *)VoidPtr; 
	//int value = (intptr_t) VoidPtr;
	/**squares the value */
	int *result = arg*arg;
	//int *final = &result;
	printf("%d\n", *result); 

	//fflush(stdout);
	pthread_exit(NULL);	
}
