#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern char* yytext;
extern int yyleng;

extern void* setup_lexer_string(char const* line);
extern void clean(void* buffer);

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

char* ler_comando();
char** parse_comando(char*);
int yywrap();

#endif
