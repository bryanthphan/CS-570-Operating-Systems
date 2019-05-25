#include <stdio.h>
#include "worker.h" 
#include <stdlib.h>

int main(int argc, char *argv[]) {
	/** arguments 0 through 4 */
	int NUM_THREADS = 5;
	pthread_t thread[NUM_THREADS]; 
	int i = 0;
	/**loop that spawns 5 threads 
	* pass NULL which tells POSIX threads to just use the defaults 
	*/
	//pthread_attr_t attributes;
	//pthread_attr_init(&attributes);
	//pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_JOINABLE);
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_create (&thread[i], NULL, &worker, (void *)&i);
	}
	/**waits until all threads have completed */
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_join(thread[i], NULL);
	}
	printf("work complete");
	pthread_exit(0);
}
