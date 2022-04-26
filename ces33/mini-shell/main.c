#include <stdio.h>
#include <stdbool.h>
#include "minishell.h"

int main(){
  char *command, **tokens;
  int size_command;
  clear_console();
  while(true){
    printf(GREEN("$ "));
    size_command = 0;
    command = read_command();
    tokens = parse_command(command, &size_command);

    spawn_processes(tokens, size_command);

    free(tokens);
    free(command);
  }
  return 0;
}
