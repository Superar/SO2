#include "processes_manager.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void insere_processo(Processo *p)
{
  if(lista_proc == NULL) {
    lista_proc = p;
  }
  else if (busca_proc_pid(p->pid) == NULL)
  {
    Processo* proc_atual = lista_proc;
    while (proc_atual->proximo != NULL)
    {
      proc_atual = proc_atual->proximo;
    }
    proc_atual->proximo = p;
    p->anterior = proc_atual;
  }
}

Processo* busca_proc_pid(pid_t pid)
{
  if(lista_proc == NULL)
  {
    return NULL;
  }
  else
  {
    Processo* proc_atual = lista_proc;
    while ((proc_atual->proximo != NULL)&&(pid != proc_atual->pid))
    {
      proc_atual = proc_atual->proximo;
    }

    if(pid == proc_atual->pid)
    {
      return proc_atual;
    }

    return NULL;
  }
}

Processo* busca_proc_num_job(int num_job)
{
  if(lista_proc == NULL)
  {
    return NULL;
  }
  else
  {
    Processo* proc_atual = lista_proc;
    int i;
    for(i = 1; (i < num_job) && (proc_atual != NULL); i++)
    {
      proc_atual = proc_atual->proximo;
    }
    return proc_atual;
  }
}

void atualiza_status_processo(pid_t pid, int status) {
  Processo* p = busca_proc_pid(pid);
  if(p != NULL)
  {
    p->status = status;
  }
}

int retira_processo(pid_t pid)
{
  if(lista_proc == NULL) {
    return -1;
  }
  else {
    Processo* proc_atual = lista_proc;
    while ((proc_atual->proximo != NULL)&&(pid != proc_atual->pid))
    {
      proc_atual = proc_atual->proximo;
    }

    if(pid == proc_atual->pid)
    {
      if(proc_atual->anterior == NULL)
      {
        lista_proc = proc_atual->proximo;
        if(lista_proc != NULL)
        {
          lista_proc->anterior = NULL;
        }
      }
      else
      {
        proc_atual->anterior->proximo = proc_atual->proximo;

        if(proc_atual->proximo != NULL)
        {
          proc_atual->proximo->anterior = proc_atual->anterior;
        }
      }
      free(proc_atual);
      return 0;
    }
    else
    {
      return -1;
    }
  }
}

void print_args(char** args)
{
  int i;
  char* arg = args[0];

  for(i = 1; arg != NULL; arg = args[i++])
  {
    printf("%s ", arg);
  }
}

void jobs()
{
  if(lista_proc != NULL) {
    Processo* proc_atual = lista_proc;
    Processo* proximo_proc;
    char status[10];

    int i;
    for(i = 1; proc_atual != NULL; i++)
    {
      if(proc_atual->status == RUNNING)
      {
        strcpy(status, "RUNNING");
      }
      else if (proc_atual->status == STOPPED)
      {
        strcpy(status, "STOPPED");
      }
      else
      {
        strcpy(status, "DONE");
      }

      printf("[%d] %s\t%d\t", i, status, proc_atual->pid);
      print_args(proc_atual->args);
      printf("\n");

      proximo_proc = proc_atual->proximo;

      if(proc_atual->status == DONE)
      {
        retira_processo(proc_atual->pid);
      }
      proc_atual = proximo_proc;
    }
  }
}

void configura_redir(Processo* p, Comando* comando)
{
  if(comando->out != NULL) {
    int flags = O_WRONLY|O_CREAT;

    if(comando->out_option > 1) {
      flags |= O_APPEND;
    }

    int fd = open(comando->out, flags, 00644);

    if(fd < 0) {
      perror("Impossivel abrir arquivo");
      exit(EXIT_SUCCESS);
    }

    dup2(fd, STDOUT_FILENO);
    close(fd);
  }

  if(comando->in != NULL) {

    int fd = open(comando->in, O_RDONLY);

    if(fd < 0) {
      perror("Impossivel abrir arquivo");
      exit(EXIT_SUCCESS);
    }

    dup2(fd, STDIN_FILENO);
    close(fd);
  }

  if(comando->err != NULL) {
    int flags = O_WRONLY|O_CREAT;

    if(comando->err_option > 1) {
      flags |= O_APPEND;
    }

    int fd = open(comando->err, flags);

    if(fd < 0) {
      perror("Impossivel abrir arquivo");
      exit(EXIT_SUCCESS);
    }

    dup2(fd, STDERR_FILENO);
    close(fd);
  }
}

int inicia_processo(Processo** p, Comando* comando)
{
  Processo* proc_atual;
  Comando* comando_atual = comando;
  int pipefd[2];
  int stdin = (int) STDIN_FILENO;
  int stdout;

  while(comando_atual != NULL)
  {
    proc_atual = malloc(sizeof(Processo));

    if (comando_atual->pipe) {
      if(pipe(pipefd) < 0) {
          perror("Erro pipe");
          exit(EXIT_SUCCESS);
      }
      stdout = pipefd[1];
    } else {
      stdout = (int) STDOUT_FILENO;
    }

    proc_atual->args = comando_atual->args;
    proc_atual->status = RUNNING;
    proc_atual->pid = fork();

    if (proc_atual->pid < 0)
    {
      fprintf(stderr, "Erro na execu%c%co do fork\n", 135, 198);
      exit(EXIT_FAILURE);
    }
    else if (proc_atual->pid == 0) // filho
    {
      if(stdin != (int) STDIN_FILENO)
      {
        if(dup2(stdin,STDIN_FILENO) < 0)
        {
          perror("erro in");
          exit(EXIT_FAILURE);
        }
        close(stdin);
      }

      if(stdout != (int) STDOUT_FILENO)
      {
        if(dup2(stdout, STDOUT_FILENO) < 0)
        {
          perror("erro out");
          exit(EXIT_FAILURE);
        }
        close(stdout);
      }

      configura_redir(proc_atual,comando_atual);
      execvp(proc_atual->args[0], proc_atual->args);
      fprintf(stderr, "%s: comando não encontrado\n", comando_atual->args[0]);
      exit(EXIT_FAILURE);
    }

    if (comando_atual->pipe) {
      stdin = pipefd[0];
      close(stdout);
    }
    else if(stdin != (int) STDIN_FILENO) {
      close(stdin);
    }

    comando_atual = comando_atual->next;
  }

  *p = proc_atual;

}

void coloca_background(Processo* p)
{
  if(!kill(p->pid, SIGCONT))
  {
    p->status = RUNNING;
  }
}

void coloca_foreground(Processo* p)
{
  int status;
  fg_proc = p;
  fg_proc->status = RUNNING;

  if(kill(p->pid, SIGCONT) == 0)
  {
    waitpid(fg_proc->pid, &status, WUNTRACED);
    if(WIFSTOPPED(status))
    {
      fg_proc->status = STOPPED;
      insere_processo(fg_proc);
    }
    else
    {
      if(retira_processo(fg_proc->pid) < 0)
      {
        free(fg_proc);
      }
    }
    fg_proc = NULL;
  }
  else
  {
    perror("Erro no envio de sinal");
  }
}

int executar_comando(Comando* comando)
{
  if (!verifica_builtins(comando))
  {
    Processo* p = NULL;

    inicia_processo(&p, comando);

    if(!comando->bg)
    {
      coloca_foreground(p);
    }
    else
    {
      insere_processo(p);
    }
  }
  return 0;
}

int verifica_builtins(Comando* comando)
{
  if (!strcmp(comando->args[0], "exit"))
  {
    exit(EXIT_SUCCESS);
  }
  else if(!strcmp(comando->args[0], "jobs"))
  {
    jobs();
  }
  else if (!strcmp(comando->args[0], "cd"))
  {
    if (comando->args[1])
    {
      if(chdir(comando->args[1]) == -1)
      {
        fprintf(stderr, "cd: %s: %s\n", comando->args[1], strerror(errno));
      }
    }
    else
    {
      struct passwd *pw = getpwuid(getuid());
      chdir(pw->pw_dir);
    }
  }
  else if (!strcmp(comando->args[0], "bg"))
  {
    if(comando->args[1] != NULL)
    {
      int nro;
      Processo* p = NULL;

      if(comando->args[1][0] == '%')
      {
        nro = atoi(comando->args[1] + sizeof(char));
        p = busca_proc_num_job(nro);
      }
      else
      {
        nro = atoi(comando->args[1]);
        p = busca_proc_pid(nro);
      }

      if(p == NULL)
      {
        printf("%s\n", "Job inexistente");
      }
      else
      {
        print_args(p->args);
        printf("\n");
        coloca_background(p);
      }
    }
  }
  else if (!strcmp(comando->args[0], "fg"))
  {
    if(comando->args[1] != NULL)
    {
      int nro;
      Processo* p = NULL;

      if(comando->args[1][0] == '%')
      {
        nro = atoi(comando->args[1] + sizeof(char));
        p = busca_proc_num_job(nro);
      }
      else
      {
        nro = atoi(comando->args[1]);
        p = busca_proc_pid(nro);
      }

      if(p == NULL)
      {
        printf("%s\n", "Job inexistente");
      }
      else
      {
        print_args(p->args);
        printf("\n");
        coloca_foreground(p);
      }
    }
  }
  else
  {
    return 0;
  }
  return 1;
}
