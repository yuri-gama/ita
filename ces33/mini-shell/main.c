#include <stdio.h>
#include <stdbool.h>
#include "minishell.h"

int main(){
  char *command;
  process *lprocess = NULL;
  job running_job;

  clear_console();
  while(true){
    printf(GREEN("$ "));
    running_job.size_command = 0;
    command = read_command();
    lprocess = parse_command(command, &running_job.size_command);
    
    spawn_processes(lprocess, &running_job);

    free_process(lprocess);
    free(command);
  }
  return 0;
}
