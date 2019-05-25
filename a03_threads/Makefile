CC = g++

thread_demo: thread_demo.cpp worker.cpp worker.h
	$(CC) -o thread_demo thread_demo.cpp worker.cpp -lrt -lpthread

clean: find . -name '*.o' -delete
