#include "minishell.h"

void clear_console(){
  pid_t pid_child = fork();
  int status;
  char *args[] = {"/bin/clear", NULL};
  if(pid_child == 0){
    execv("/bin/clear", args);
    perror("execv");
  }

  wait(&status);
}

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

process* parse_command(char *str, int *i){
  char* pt = str;
  process *lprocess, *p;

  pt = strtok(str, "|");

  while(pt != NULL){
    (*i)++;
    if(*i == 1){
      lprocess = (struct process *) malloc(sizeof (struct process) );
      p = lprocess;
    }
    else{
      p->next = (struct process *) malloc(sizeof (struct process) );
      p = p->next;
    }

    p->token = pt;  

    pt = strtok(NULL, "|");
  }

  p->next = NULL;

  return lprocess;
}

bool spawn_processes(process *tokens, job *rjob){
  char *stdout_fd, *stdin_fd;
  int **pipes = (int **) malloc(sizeof(int *));
  process *pt;
  
  rjob->first_process = tokens;
  pt = rjob->first_process;

  for(int count = 0; count < rjob->size_command; count++){
        
    if(count + 1 < rjob->size_command){
      pipes = (int **) realloc(pipes, (count+1)*sizeof(int *));
      pipes[count] = (int *) malloc(2*sizeof(int));
      pipe(pipes[count]);
    }
    
    pt->all = get_all(pt->token);
    pt->argv = get_args(pt->all);
    pid_t pid_child = fork();
    if(pid_child == -1)
      perror("fork");

    if(pid_child != 0){
      wait(&pt->status);
    }

    if(true){
      // stdin logic
      if(count == 0){
       stdin_fd = get_stdin(pt->all);
        
        if(stdin_fd != NULL && pid_child == 0){
          rjob->stdin = open(stdin_fd, O_RDONLY);
          dup2(rjob->stdin, STDIN_FILENO);
          close(rjob->stdin);
        }
      }
    else{
        if(pid_child == 0)
          dup2(pipes[count-1][0], STDIN_FILENO);
        close(pipes[count-1][0]);
      }

      // stdout logic
      if(count + 1 == rjob->size_command){
        stdout_fd = get_stdout(pt->all);
        if(stdout_fd != NULL && pid_child == 0){
          rjob->stdout = open(stdout_fd, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
          dup2(rjob->stdout, STDOUT_FILENO);
          close(rjob->stdout);
        }
      }
    else{
        if(pid_child == 0)
          dup2(pipes[count][1], STDOUT_FILENO);
      }
        if(count + 1 != rjob->size_command)
          close(pipes[count][1]);

      if(pid_child == 0){
        execv(pt->all[0], pt->argv);
        perror("execve");
      }
     
    }
    pt = pt->next;
  }

        
  if(rjob->size_command > 1)
    for(int i = 0; i < rjob->size_command-1; i++)
      free(pipes[i]);

  free(pipes);

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

void free_process(process *pt){
  free_process_pt(pt);
  pt = NULL;
}

void free_process_pt(process * pt){
  if(pt == NULL){
    return;
  }
  free_process_pt(pt->next);
  free(pt->argv);
  free(pt->all);
  free(pt);
}

/* Find the active job with the indicated pgid.  */
job *find_job (pid_t pgid, job * first_job)
{
  job *j;

  for (j = first_job; j; j = j->next)
    if (j->pgid == pgid)
      return j;
  return NULL;
}

/* Return true if all processes in the job have stopped or completed.  */
int job_is_stopped (job *j)
{
  process *p;

  for (p = j->first_process; p; p = p->next)
    if (!p->completed && !p->stopped)
      return 0;
  return 1;
}

/* Return true if all processes in the job have completed.  */
int job_is_completed (job *j)
{
  process *p;

  for (p = j->first_process; p; p = p->next)
    if (!p->completed)
      return 0;
  return 1;
}