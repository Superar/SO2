#ifndef PROCESSES_MANAGER_H
#define PROCESSES_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "command_parser.h"

#define MIN_TAM_PROCESS_TABLE 5

typedef struct Processo
{
  pid_t pid;
  char **args;
  int status;
} Processo;

Processo** tabela_proc;
int tam_tabela_proc;
int cur_index_tabela_proc;

void init_process_manager();

int verifica_builtins(Comando*);

int executar_comando(Comando*);

#endif
