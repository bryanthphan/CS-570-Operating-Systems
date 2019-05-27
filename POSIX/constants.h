#pragma once
#include <semaphore.h>
#include <string>
#include <vector>

using namespace std;

const int BUFFER_SIZE = 10;
const int FROG = 1;
const int ESCARGOT = 2;
const int CLEAR = 0;
const int TRUE = 0;
const int MAX = 100;
const int MAX_NUM_FROGS = 3;
const int BAD_ARGUMENT = 33;

const string FROG_STR = "crunchy frog bite";
const string ESCARGOT_STR = "escargot sucker";

//This struct contains all of our shared semaphores
//and other information between threads

struct mizzo_info {
  sem_t *full;
  sem_t *empty;
  sem_t *access_buffer;
  sem_t *num_frogs;
  int *total_consumed;
  int *total_produced;
  int *frogs_produced;
  int *escargot_produced;
  sem_t *to_consume;
  sem_t *to_produce;
  int *next_in;
  int *next_out;
  vector<int> *buffer;
};

//This struct holds the information we will
//pass into our producer thread instances
struct prod_struct {
  int candy_type;
  int wait_time;
  mizzo_info info;
};

//This struct holds the information we will
//pass into our consumer thread instances
struct consm_struct {
  string consm_name;
  int wait_time;
  mizzo_info info;
};

//This struct holds information we will
//return from our consumers after we are done
struct tot_consm_struct {
  int frogs;
  int escargots;
};
