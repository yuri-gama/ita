#include <stdio.h>
#include <stdbool.h>
#include "minishell.h"

int main(){
  char *command;
  process *lprocess = NULL;
  job running_job;
  int size_command;
  clear_console();
  while(true){
    printf(GREEN("$ "));
    size_command = 0;
    command = read_command();
    lprocess = parse_command(command, &size_command);
    
    spawn_processes(lprocess, size_command, &running_job);

    free_process(lprocess);
    free(command);
  }
  return 0;
}
