#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern int yylex();
extern char* yytext;

extern int yyleng;

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

// Estrutura resultante da analise de uma string digitada no terminal
typedef struct Comando{
  char** args; // argumentos do programa, incluindo o proprio nome do programa
  int nro_args; // quantidade de argumentos passados
  char* in; // arquivo de redirecionamento de entrada (<)
  char* out; // arquivo de redirecionamento de saida (> ou >>)
  int out_option; // indicador do tipo de redirecionamento de saida
  char* err; // arquivo de redirecionamento de saida de erro (2> ou 2>>)
  int err_option; // indicador do tipo de redirecionamento de saida de erro
  int pipe; // indicador da presenca de comunicacao em pipe
  int bg; // indicador da opcao em background
  struct Comando* next; // ponteiro para o proximo comando (outro lado do pipe)
} Comando;

// Inicializador da estrutura Comando
Comando* init_comando();

// Escreve na tela o diretorio atual
void print_pwd();

// Le do terminal e retorna uma string com o conteudo lido
char* ler_comando();

// Analisa lexica e sintaticamente a string lida e retorna o comando
Comando* parse_comando(char*);

int yywrap();

#endif
