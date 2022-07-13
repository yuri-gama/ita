#ifndef MINISHELL
#define MINISHELL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <fcntl.h>

#define S_IRUSR 00400
#define S_IWUSR 00200

// color minishell
#define GREEN(str) "\x1b[95m" str "\x1b[0m"

typedef struct process{
   struct process *next;
   char *token;
   char **argv;
   char **all;
   int stdin, stdout, status;
} process;

typedef struct job{
  struct job *next;
  char *command;
  process *first_process;
  pid_t pgid;
  char notified;
//  struct termios tmodes;
  int stdin, stdout;
} job;

void clear_console();

char* read_command(); // Get input from command line

process* parse_command(char *, int *); // Parse command line into tokens, separator: '|'

bool spawn_processes(process*, int, job *); // Spawn each token as a proccess

char** get_all(char *); // It breaks token by white spaces
char ** get_args(char **); // It gets args with args[0] = path && arg[last] = NULL
char * get_stdout(char **); // It gets path to create FD_stdout
char * get_stdin(char **); // It gets path to create FD_stdin

void free_process(process *);

void free_process_pt(process *);

#endif
