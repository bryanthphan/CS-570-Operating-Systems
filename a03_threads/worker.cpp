#include <stdio.h> 
#include <pthread.h> 
#include <stdint.h>

extern "C" void *worker(void *VoidPtr) {
	/**converts void pointer to a value of int */
	int arg = (intptr_t)VoidPtr; 
	/**squares the value */
	int value = arg*arg;
	printf("%d\n", value); 

	//fflush(stdout);
	pthread_exit(NULL);	
}
