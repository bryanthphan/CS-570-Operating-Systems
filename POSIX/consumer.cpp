#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <stdint.h>
#include <semaphore.h>
#include "constants.h"
#include <unistd.h>
#include "producer.h"
#include <string>
#include "consumer.h"

using namespace std;

extern "C" void* consumer(void* consm_args) {
  //Grab the argument structure
  consm_struct *args = (struct consm_struct*)consm_args;

  //Get name of our consumer
  string consm_name = args->consm_name;

  //Grab struct containing share information
  //between our threads
  mizzo_info info = args->info;

  //Grab the buffer(conveyor belt)
  vector<int> *buffer = info.buffer;

  int frogs_consumed_by_me = 0;
  int escargot_consumed_by_me = 0;
  bool wait = true;

  //if our wait time is 0, we incur no wait time
  int wait_time = args->wait_time;
  if (wait_time <= 0)
    wait = false;

  do {
    //Check if there is more candies to be consumed
    int keep_going = sem_trywait(info.to_consume);
    if(keep_going == TRUE) {
      //Wait till there's a candy available
      sem_wait(info.full);
      //Now wait for buffer access
      sem_wait(info.access_buffer);

      //START CRITICAL SECTION

      //remove from buffer
      int removed = buffer->at(*(info.next_out));
      buffer->at(*(info.next_out)) = clear;
      //Update the buffer index
      *(info.next_out) = (*(info.next_out) + 1) %BUFFER_SIZE;

      //Collect the name of the candy we ate and update our counts
      string candy = "";
      if(removed == FROG) {
        candy = FROG_STR;
        //If we ate a frog we give a frog count back
        sem_post(info.num_frogs);
        *(info.frogs_produced) = *(info.frogs_produced) - 1;
        frogs_consumed_by_me++;
      }
      else {
        candy = ESCARGOT_STR;
        *(info.escargot_produced) = *(info.escargot_produced) - 1;
        escargot_consumed_by_me++;
      }

      //Empty a slot
      sem_post(info.empty);

      //Increment other counts
      *(info.total_consumed) = *(info.total_consumed) + 1;
      int frogs_and_escargot = *(info.frogs_produced) + *(info.escargot_produced);

      //Print our report
      cout << "Belt: " << *(info.frogs_produced) << " frogs + " <<
        *(info.escargot_produced) << " escargots = " << frogs_and_escargot
        << ".produced: " << *(info.total_produced) << "     "
        << consm_name << " consumed " << candy << endl;

      //END CRITICAL SECTION
      //Release buffer access
      sem_post(info.access_buffer);

      //Take a nap
      if(wait)
        usleep(wait_time);
    }
    else {
      //We are done producing, so set our counts
      //in our return structure and exit
      tot_consm_struct *ret_val = new tot_consm_struct;
      ret_val->escargots = escargot_consumed_by_me;
      ret_val->frogs = frogs_consumed_by_me;

      pthread_exit((void *)ret_val);
    }
  }
  while(true); 
}
