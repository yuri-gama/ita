#include "minishell.h"

char* read_command(){
  char *str, c;
  int i = 1;
  str = (char *) malloc(sizeof(char));
  while(true){
   scanf("%c", &c);
    if(c == '\n')
      break;

    str = (char *) realloc(str, sizeof(char)*i);
    str[i-1] = c;
    i++;
  }
  
  str = (char *) realloc(str, sizeof(char)*i);
  str[i-1] = '\0';

  return str;
}

char** parse_command(char *str, int *i){
  char* pt = str;
  char **command = NULL;

  pt = strtok(str, "|");

  while(pt != NULL){
    (*i)++;
    if(*i == 1)
      command = (char **) malloc(sizeof(char *));
    else
      command = (char **) realloc(command, sizeof(char *)*i[0]);
    
    command[*i-1] = pt;
    pt = strtok(NULL, "|");
  }

  return command;
}

bool spawn_processes(char **tokens, int size_tokens){
  char **all, **argv;
  char *stdout_fd, *stdin_fd;
  int **pipes = (int **) malloc(sizeof(int *));
  int FD_stdout, FD_stdin;

  int *status = (int *) malloc(sizeof(int)*size_tokens);
  
  for(int count = 0; count < size_tokens; count++){
    
    if(count + 1 < size_tokens){
      pipes = (int **) realloc(pipes, (count+1)*sizeof(int *));
      pipes[count] = (int *) malloc(2*sizeof(int));
      pipe(pipes[count]);
    }
    
    all = get_all(tokens[count]);
    argv = get_args(all);
    pid_t pid_child = fork();
    if(pid_child == -1)
      perror("fork");

    if(pid_child != 0){
      wait(&status[count]);
    }

    if(true){
      // stdin logic
      if(count == 0){
       stdin_fd = get_stdin(all);
        
        if(stdin_fd != NULL && pid_child == 0){
          FD_stdin = open(stdin_fd, O_RDONLY);
          dup2(FD_stdin, STDIN_FILENO);
          close(FD_stdin);
        }
      }
    else{
        if(pid_child == 0)
          dup2(pipes[count-1][0], STDIN_FILENO);
        close(pipes[count-1][0]);
      }

      // stdout logic
      if(count + 1 == size_tokens){
        stdout_fd = get_stdout(all);
        if(stdout_fd != NULL && pid_child == 0){
          FD_stdout = open(stdout_fd, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
          dup2(FD_stdout, STDOUT_FILENO);
          close(FD_stdout);
        }
      }
    else{
        if(pid_child == 0)
          dup2(pipes[count][1], STDOUT_FILENO);
      }
        if(count + 1 != size_tokens)
          close(pipes[count][1]);

      if(pid_child == 0){
        execv(all[0], argv);
        perror("execve");
      }
     
    }
 
  }

        
  if(size_tokens > 1)
    for(int i = 0; i < size_tokens-1; i++)
      free(pipes[i]);

  free(pipes);
  free(status);

  return true;

}

char** get_all(char *str){
  char *pt = strtok(str, " ");
  char **all_vec = (char **) malloc(sizeof(char *)); 
  int count = 0;
  while(pt != NULL){
    count++;
    all_vec = realloc(all_vec, sizeof(char *)*count);
    all_vec[count-1] = pt;
    pt = strtok(NULL, " ");
  }

  all_vec = realloc(all_vec, sizeof(char *)*(count+1));
  all_vec[count] = NULL;
  return all_vec;
}

char ** get_args(char** all_vec){
  int i = 0;
  char **args = (char **) malloc(sizeof(char *));
  while(all_vec[i] != NULL){
    if(!strcmp(all_vec[i],"<") || !strcmp(all_vec[i],">")){
      break;
    }
    args = realloc(args, sizeof(char *)*(i+1));
    args[i] = all_vec[i];
    i++;
  }

  args = realloc(args, sizeof(char *)*(i+1));
  args[i] = NULL;
  return args;
}


char* get_stdout(char **str){
  int i = 0;

  while(str[i] != NULL){
    if(strcmp(str[i++], ">") == 0)
      return str[i];
  }
  
  return NULL;
}

char * get_stdin(char **str){
 int i = 0;
  while(str[i] != NULL)
    if(strcmp(str[i++], "<") == 0)
      return str[i];
  
  return NULL;
}
