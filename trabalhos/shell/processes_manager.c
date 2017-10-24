#include "processes_manager.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void init_process_manager()
{
  tabela_proc = malloc(MIN_TAM_PROCESS_TABLE * sizeof(Processo*));
  tam_tabela_proc = MIN_TAM_PROCESS_TABLE;
  cur_index_tabela_proc = -1;
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
    printf("%s\n", "jobs hein");
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
      Processo p;

      p.pid = fork();
      p.args = comando->args;
      int status;

      if (p.pid < 0)
      {
          fprintf(stderr, "Erro na execu%c%co do fork\n", 135, 198);
          exit(EXIT_FAILURE);
      }
      else if (p.pid == 0) // filho
      {
          configura_processo(&p,comando);
          exit(EXIT_SUCCESS);
      }
      else // pai
      {
          if(!comando->bg)
            waitpid(p.pid, &status, 0);
          return 0;
      }
    }

    return 0;
}
