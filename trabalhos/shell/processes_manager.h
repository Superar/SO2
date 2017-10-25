#ifndef PROCESSES_MANAGER_H
#define PROCESSES_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "command_parser.h"

typedef struct Processo
{
  pid_t pid;
  char **args;
  int status;
  struct Processo* proximo;
  struct Processo* anterior;
} Processo;

Processo* lista_proc;

void insere_processo(Processo*);

Processo* busca_processo(pid_t);

int retira_processo(pid_t);

int verifica_builtins(Comando*);

int executar_comando(Comando*);

#endif
