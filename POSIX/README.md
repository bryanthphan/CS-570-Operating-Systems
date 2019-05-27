Assignment details (Paired Programming): 
Suppose that Lucy and Ethel have gone to work for the Mizzo candy factory.
Mizzo produces two types of candy: crunchy frog bites and everlasting escargot
suckers. Unlike their last job, Mizzo has automatic flow control on their assembly
line. No more than 10 candies are on the conveyer belt at any given time.
Crunchy frog bites tend to be more expensive than escargot suckers, so Mizzo has
implemented a policy to prevent putting too many frog bites in the same box. No
more than 3 frog bites can be on the conveyer belt at any given time. (Candies
are taken off the line in the order that they are produced.)
Write a program using POSIX unnamed semaphores and POSIX threads to simulate this
multiple producer and multiple consumer problem. POSIX unnamed semaphores are
covered in the frequently asked questions section of the course web site.
Your program must meet the following design criteria:

1. The program should take the following optional command line arguments:
  -E N Specifies the number of milliseconds N that the Ethel consumer requires
  to put a candy in the box and should be invoked each time Ethel
  removes a candy regardless of the candy type.
  -L N Similar argument for the Lucy consumer.
  -f N Specifies the number of milliseconds between the production of each
  crunchy frog bite.
  -e N Specifies the number of milliseconds between the production of each
  everlasting escargot sucker.
  
If an argument is not given for any one of the threads, that thread should incur no
delay. The class FAQ explains command line argument parsing and how to cause
a thread to sleep for a given interval (remember from the last assignment that
using getopt can make parsing much easier). You need not check for errors when
sleeping.
2. Your program should written using multiple files that have some type of logical
coherency (e.g. producer, consumer, belt, etc.). Write your Makefile (required
with all programs) early, this will permit you to not have to worry about which
files have changed since the last time you compiled. The Makefile should
generate program mizzo (lower case) and be in directory cs570/a05 on your
edoras account.
Roch p. 4
3. Do not use global variables to communicate information to your threads. Pass in
data structures.
4. Each candy generator should be written as a separate thread. The consumer
processes (Lucy & Ethel) must share common code but must be executed as
separate threads. It is also possible to share common code for the producers, but
not required.
5. One of the elements of the data structure that you pass to your consumer threads
should be the consumer thread name (Lucy or Ethel), this will allow you to print
messages indicating whether Lucy or Ethel did the work.
6. Maintain the ordering of the candy production and consumption. Candies are
removed in first-in first-out order.
7. Your producers should stop production once 100 candies are produced. After all
100 candies are consumed, the program should exit. Descriptive output should be
produced each time a candy is added or removed from the conveyer belt. When
the day’s candy production is complete, you should print out how many of each
type of candy was produced and how many of each type were processed by each
of the consumer threads. 

How to run: 
First type the "make" command on your terminal 
Then type "mizzo -f 600 -e 400 -L 300 -E 500"
