#include <stdio.h>
#include <stdbool.h>
#include "minishell.h"

extern bool termination_prog = false;

int main(){
  char *command;
  process *lprocess = NULL;
  job running_job;

  // init_shell();

  // signal (SIGINT, stop_handler);

  // while(true){
    printf(GREEN("$ "));
    running_job.size_command = 0;
    command = read_command();
    printf("passou");
    if(!termination_prog){
      lprocess = parse_command(command, &running_job.size_command);
      printf("spawning");
      spawn_processes(lprocess, &running_job);

      printf("algo aqui");

      free_process(lprocess);
    }
    free(command);
    termination_prog = false;
  // }

  printf("CHEGUEI AO FINAL");
  return 0;
}
