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
  else {
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
    while ((proc_atual->proximo != NULL)&&(pid < proc_atual->pid))
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
    for(i = 0; (i < num_job) && (proc_atual != NULL); i++)
    {
      proc_atual = proc_atual->proximo;
    }

    return proc_atual;
  }
}

int retira_processo(pid_t pid)
{
  if(lista_proc == NULL) {
    return -1;
  }
  else {
    Processo* proc_atual = lista_proc;
    while ((proc_atual->proximo != NULL)&&(pid < proc_atual->pid))
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

void jobs() {
  if(lista_proc != NULL) {
    Processo* proc_atual = lista_proc;
    char status[10];

    int i;
    for(i = 1; proc_atual != NULL; i++)
    {
      if(proc_atual->status == RUNNING)
      {
        strcpy(status, "RUNNING");
      }
      printf("[%d] %s\t%d\t%s\n", i, status, proc_atual->pid, proc_atual->args[0]);

      if(proc_atual->status == 0)
      {

      }

      proc_atual = proc_atual->proximo;
    }
  }
}

void configura_processo(Processo* p, Comando* comando)
{
  if(comando->out != NULL) {
    int flags = O_WRONLY|O_CREAT;

    if(comando->out_option > 1) {
      flags |= O_APPEND;
    }

    int fd = open(comando->out, flags);

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

  execvp(p->args[0], p->args);
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
  else
  {
    return 0;
  }
  return 1;
}

int executar_comando(Comando* comando)
{
    if (!verifica_builtins(comando))
    {
      Processo* p = malloc(sizeof(Processo));

      p->pid = fork();
      p->args = comando->args;
      p->status = RUNNING;
      int status;

      if (p->pid < 0)
      {
          fprintf(stderr, "Erro na execu%c%co do fork\n", 135, 198);
          exit(EXIT_FAILURE);
      }
      else if (p->pid == 0) // filho
      {
          configura_processo(p,comando);
          exit(EXIT_SUCCESS);
      }
      else // pai
      {
          if(!comando->bg)
          {
            waitpid(p->pid, &status, 0);

            if(WIFSTOPPED(status)){
              insere_processo(p);
            }
            else
            {
              free(p);
            }
          }
          else
          {
            insere_processo(p);
          }
          return 0;
      }
    }
    return 0;
}
