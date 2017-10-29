#ifndef PROCESSES_MANAGER_H
#define PROCESSES_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <pwd.h>
#include <errno.h>
#include "command_parser.h"

// Possiveis estados de um processo (job)
#define DONE 0
#define RUNNING 1
#define STOPPED 2

// Estrutura mantida pelo shell para o gerenciamento de seus processos filhos
// Representa uma lista duplamente encadeada dos processos
typedef struct Processo
{
    pid_t pid;
    char **args;    // Argumentos passados para o exec
    int status;     // Estado do processo (STOPPED, DONE, RUNNING)
    struct Processo* proximo; // Proximo processo na lista
    struct Processo* anterior; // Processo anterior na lista
} Processo;

// Lista de processos
Processo* lista_proc;
// Processo atual em foreground
Processo* fg_proc;

void insere_processo(Processo*);

Processo* busca_processo(pid_t);

// Modifica o estado de um processo cujo pid foi passado como parametro
// pelo estado tbm passado como parametro
void atualiza_status_processo(pid_t, int);

int retira_processo(pid_t);

// Verifica se o comando digitado se trata de um builtin
// Se for, executa o comando e retorna true, caso contrario, retorna false
int verifica_builtins(Comando*);


int executar_comando(Comando*);

Processo* busca_proc_pid(pid_t);
#endif
