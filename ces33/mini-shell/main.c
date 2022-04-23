#include <stdio.h>
#include <stdbool.h>
#include "minishell.h"

int main(){
  char *command, **tokens;
  int size_command;
  while(true){
    // printf("star while\n");
    size_command = 0;
    command = read_command();
    tokens = parse_command(command, &size_command);

    spawn_processes(tokens, size_command);

    free(tokens);
    free(command);
    //wait_and_free(command);
  }
  return 0;
}
