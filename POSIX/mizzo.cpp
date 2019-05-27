#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <stdint.h>
#include <semaphore.h>
#include "constants.h"
#include "producer.h"
#include "consumer.h"
#include <string>
#include <cstring>
#include <vector>

using namespace std;

sem_t full;
sem_t empty;
sem_t access_buffer;
sem_t num_frogs;
sem_t to_consume;
sem_t to_produce;

int next_in;
int next_out;
int frogs_produced;
int total_consumed;
int total_produced;
int escargot_produced;

int main(int argc, char* argv[]) {

  vector<int> *buffer = new vector<int>(BUFFER_SIZE);

  //Start argument looping at 1
  int startIndex = 1;
  //Initialize our wait times
  int ethel_wait = 0;
  int lucy_wait = 0;
  int frog_wait = 0;
  int escargot_wait = 0;

  //Here we will loop through our arguments
  for (int i = startIndex; i <argc; i++) {
    //Number of milliseconds for Ethel
    if (strcmp(argv[i],"-E") == 0) {
      ethel_wait = stoi(argv[i+1]);
      i++;
    }
    //Number of milliseconds for Lucy
    else if(strcmp(argv[i],"-L") == 0) {
      lucy_wait = stoi(argv[i+1]);
      i++;
    }
    //Number of milliseconds for frog producer
    else if(strcmp(argv[i],"-f") == 0) {
      frog_wait = stoi(argv[i+1]);
      i++;
    }
    //Number of milliseconds for escargot producer
    else if(strcmp(argv[i],"-e") == 0) {
      escargot_wait = stoi(argv[i+1]);
      i++;
    }
    //If we get here, we got a bad argument
    else {
      cout << "Bad argument. Returned code "
        << BAD_ARGUMENT << endl;
      exit(BAD_ARGUMENT);
    }
  }

  //Initialize production and consumption counts
  total_consumed = 0;
  total_produced = 0;
  frogs_produced = 0;
  escargot_produced = 0;

  //semaphore full should be initialized to 0
  //so that if a consumer tries to grab from the
  //buffer before it has an item they will be blocked
  sem_init(&full,0,0);
  //semaphore empty should be initialized to the capacity
  //of the buffer so that producers can fill the whole
  //buffer
  sem_init(&empty,0,BUFFER_SIZE);
  //Semaphore access_buffer keeps track of what thread
  //is currently accessing the buffer. Only one thread
  //can add or remove items from the buffer at a time
  sem_init(&access_buffer,0,1);
  //semaphore num_frogs keeps track of how many frogs
  //are currently in the buffer. Only 3 frogs can be in
  //the buffer at any given time
  sem_init(&num_frogs,0,MAX_NUM_FROGS);
  //These two semaphores will keep track of how many candies
  //we have produced and consumed, and stop us when we
  //get to MAX
  sem_init(&to_consume,0,MAX);

  sem_init(&to_produce,0,MAX);
  //Initialize indices for buffer
  next_in = 0;
  next_out = 0;

  //The mizzo info structure contains all of the data
  //that will be shared between our threads. It is passed
  //as an argument when we invoke a thread
  mizzo_info info;
  info.access_buffer = &access_buffer;
  info.buffer = buffer;
  info.empty = &empty;
  info.escargot_produced = &escargot_produced;
  info.frogs_produced = &frogs_produced;
  info.full = &full;
  info.next_in = &next_in;
  info.next_out = &next_out;
  info.num_frogs = &num_frogs;
  info.total_consumed = &total_consumed;
  info.total_produced = &total_produced;
  info.to_consume = &to_consume;
  info.to_produce = &to_produce;

  //We want our main function to wait for our threads
  //our threads to return, so we must add the joinable property
  //to our thread attributes
  pthread_attr_t attributes;
  //Initialize our attributes to default settings
  pthread_attr_init(&attributes);
  //Set up our threads so they are joinable
  pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_JOINABLE);

  //Start up the frog producer thread
  prod_struct frog_args;
  frog_args.candy_type = FROG;
  frog_args.wait_time = frog_wait;
  frog_args.info = info;

  pthread_t frog_producer;
  pthread_create(&frog_producer, &attributes, &producer, (void*)&frog_args);

  //Start up the escargot producer thread
  prod_struct escargot_args;
  escargot_args.candy_type = ESCARGOT;
  escargot_args.wait_time = escargot_wait;
  escargot_args.info = info;

  pthread_t escargot_producer;
  pthread_create(&escargot_producer, &attributes, &producer, (void*)&escargot_args);

  //Start up the Ethel consumer thread
  consm_struct ethel_args;
  ethel_args.consm_name = "Ethel";
  ethel_args.wait_time = ethel_wait;
  ethel_args.info = info;

  pthread_t ethel;
  pthread_create(&ethel, &attributes, &consumer, (void*)&ethel_args);

  //Start up the Lucy consumer thread
  consm_struct lucy_args;
  lucy_args.consm_name = "Lucy";
  lucy_args.wait_time = lucy_wait;
  lucy_args.info = info;

  pthread_t lucy;
  pthread_create(&lucy, &attributes, &consumer, (void*)&lucy_args);

  //We will pass these variables to our threads when we join,
  //so that when our threads return they can pass their
  //return values into those variables
  int *tot_frogs_prod = new int;
  int *tot_escargot_prod = new int;
  tot_consm_struct *tot_ethel_consm = new tot_consm_struct;
  tot_consm_struct *tot_lucy_consm = new tot_consm_struct;

  //Now that our threads have been created, we need to call
  //pthread_join so that our main function waits for them to
  //finish
  pthread_join(frog_producer, (void **)&tot_frogs_prod);
  pthread_join(escargot_producer, (void**)&tot_escargot_prod);
  pthread_join(ethel, (void**)&tot_ethel_consm);
  pthread_join(lucy, (void**)*tot_lucy_consm);

  //Print the production report
  cout << "\nPRODUCTION REPORT" << endl;
  cout << "-------------------------------------" << endl;
  cout << "crunchy frog bite producer generated " << *tot_frogs_prod << " candies" << endl;
  cout << "escargot sucker producer generated " << *tot_escargot_prod << " candies" << endl;
  cout << "Lucy consumed " << tot_lucy_consm->frogs
    << " crunchy frog bites + " << tot_lucy_consm->escargots
    << " escargot suckers = "
    << tot_lucy_consm->frogs + tot_lucy_consm->escargots << end;
  cout << "Ethel consumed " tot_ethel_consm->frogs
    << " crunchy frog bites + " << tot_ethel_consm->escargots
    << " escargot suckers = "
    << tot_ethel_consm->frogs + tot_ethel_consm->escargots << end;
}
