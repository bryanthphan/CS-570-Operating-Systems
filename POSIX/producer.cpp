#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <stdint.h>
#include <semaphore.h>
#include "constants.h"
#include <unistd.h>
#include "producer.h"
#include <string>

using namespace std;

extern "C" void* producer(void* prod_args) {
  //Grab the passed arguments
  prod_struct *args = (struct prod_struct*)prod_args;

  //Get the struct containing the shared
  //information between our threads
  mizzo_info info = args->info;

  //Grab the buffer (conveyor belt)
  vector<int> *buffer = info.buffer;

  int*produced_by_me = new int;
  bool wait = true;

  //If the wait time passed is 0, we
  //incur no wait time
  int wait_time = args->wait_time;
  if(wait_time <= 0)
    wait = false;

  string candy = "";
  if (args->candy_type == FROG) {
    candy = FROG_STR;
  }
  else if (args->candy_type == ESCARGOT) {
    candy = ESCARGOT_STR;
  }

  do {
    if(args->candy_type == FROG) {
      //If we are producing frogs, we must make sure
      //that we are allowed to first so that we do not grab
      //one of the produce counts and then just wait

      sem_wait(info.num_frogs);
    }
    //Check if there are any produce counts left
    int keep_going = sem_trywait(info.to_produce);
    if(keep_going == TRUE) {
      //Wait till we grab an empty slot
      sem_wait(info.empty);
      //Now wait for access to the buffer
      sem_wait(info.access_buffer);

      //START CRITICAL SECTION

      //place in buffer
      buffer->at(*(info.next_in)) = args->candy_type;
      //Update the buffer index
      *(info.next_in) = (*(info.next_in) + 1) %BUFFER_SIZE;

      //Update full count
      sem_post(info.full);

      //Increment all of our production counts
      if (args->candy_type == FROG) {
        *(info.frogs_produced) = *(info.frogs_produced) + 1;
      }
      else
        *(info.escargot_produced) = *(info.escargot_produced) + 1;

      *produced_by_me = *produced_by_me + 1;
      *(info.total_produced) = *(info.total_produced) + 1;

      int frogs_and_escargot = *(info.frogs_produced) + *(info.escargot_produced);

      //Print report
      cout << "Belt: " << *(info.frogs_produced) << " frogs + " <<
        *(info.escargot_produced) << "escargots = " << frogs_and_escargot
        << ". produced: " << *(info.total_produced) << "    Added "
        << candy << endl;

      //END CRITICAL SECTION
      //Release buffer access
      sem_post(info.access_buffer);

      //Now take a nap
      if(wait)
        usleep(wait_time);
    }
    else {
      //We are done producing, return
      //our production count
      pthread_exit((void *)produced_by_me);
    }
  } while (true); 
}
