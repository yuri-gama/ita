#include "mysignal.h"

void stop_handler(int signum){
  signal(signum, stop_handler); // to get the next time
  termination_prog = true;
  printf("peguei o sinal\n");
  printf("termination_prog: %d\n", termination_prog);
}