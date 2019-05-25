#include <stdio.h>
#include "worker.h" 
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
	/** arguments 0 through 4 */
	int NUM_THREADS = 5;
	int i;
	pthread_t thread[NUM_THREADS]; 
	/**loop that spawns 5 threads 
	* pass NULL which tells POSIX threads to just use the defaults 
	*/
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_create (&thread[i], NULL, &worker, (void *)(intptr_t) i);
	}
	/**waits until all threads have completed */
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_join(thread[i], NULL);
	}
	printf("work complete");
	pthread_exit(0);
}
