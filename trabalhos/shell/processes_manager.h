#ifndef PROCESSES_MANAGER_H
#define PROCESSES_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

typedef struct process
{
  pid_t pid;
  char **args;
} process;

int executar_comando(char**);

#endif
