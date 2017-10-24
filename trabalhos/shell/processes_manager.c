#include "processes_manager.h"

void init_process_manager()
{
  tabela_proc = malloc(MIN_TAM_PROCESS_TABLE * sizeof(Processo*));
  tam_tabela_proc = MIN_TAM_PROCESS_TABLE;
  cur_index_tabela_proc = -1;
}

int configura_processo(Processo* processo, Comando* comando)
{
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
          execvp(p.args[0], p.args);
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
