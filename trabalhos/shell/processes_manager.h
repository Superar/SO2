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

// Busca um processo pelo seu pid
Processo* busca_proc_pid(pid_t);

// Busca um processo pelo seu numero de job (sequencia em que foi chamado)
Processo* busca_proc_num_job(int);

// Modifica o estado de um processo cujo pid foi passado como parametro
// pelo estado tbm passado como parametro
void atualiza_status_processo(pid_t, int);

// Retira um processo da lista_proc pelo seu pid
int retira_processo(pid_t);

// Printa na tela todos os argumentos passados ao comando (incluindo seu nome)
void print_args(char**);


// Realiza o processo de execucao de um comando passado como parametro
int executar_comando(Comando*);

// Verifica se o comando digitado se trata de um builtin
// Se for, executa o comando e retorna true, caso contrario, retorna false
int verifica_builtins(Comando*);
#endif
