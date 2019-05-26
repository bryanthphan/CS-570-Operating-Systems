A small program, thread_demo, that does the following: 
  thread_demo. creates a loop that spawns 5 threads with integer arguments 0 through 4 
  The thread's entry point, in file worker.c, is function void worker(void *) where 
  void * is expected to be a pointer to an int. The thread computes and prints the square of the
  argument, then returns. 
  The main thread waits  until all threads have completed, then print "work complete." 
This program uses the POSIX thread interface 
In order to run the program enter the command "make" followed by 
