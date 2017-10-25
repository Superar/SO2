#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern int yylex();
extern char* yytext;

#ifndef YY_TYPEDEF_YY_SIZE_T
#define YY_TYPEDEF_YY_SIZE_T
typedef size_t yy_size_t;
#endif
extern yy_size_t yyleng;

extern void* setup_lexer_string(char const* line);
extern void clean(void* buffer);

#define NRO_MAX_COMANDOS 10

#define TAM_BUFFER_COMANDO 100

#define TAM_BUFFER_CAMINHO 1024

/* Tokens */
#define NEWLINE 250
#define IDENTIFICADOR 251
#define SPACE 252
#define ECOMERCIAL 253
#define MAIOR 254
#define MENOR 255
#define MAIOR2 256
#define MAIORMAIOR 257
#define MAIORMAIOR2 588
#define PIPE 289

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
int yywrap();

#endif
