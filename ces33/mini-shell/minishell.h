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

char* read_command();

char** parse_command(char *, int *);

//bool check_exit(char **);

bool spawn_processes(char **, int);

char** get_all(char *);
char ** get_args(char **);
char * get_stdout(char **);
char * get_stdin(char **);

#endif
