#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Numero maximo de comandos por linha
#define NRO_MAX_COMANDOS 10

#define TAM_BUFFER_COMANDO 100

typedef struct Comando{
  char** args;
  int nro_args;
  char* in;
  char* out;
  int out_option;
  char* err;
  int err_option;
  int pipe;
  int bg;
  struct Comando* next;
} Comando;

Comando* init_comando();

char* ler_comando();
Comando* parse_comando(char*);

#endif
