digito  [0-9]
letra   [A-Za-z]
especiais [-./~%*_]

%{
#include <string.h>
#include "command_parser.h"
%}

/* Funcoes que nao serao usadas */
%option nounput
%option noinput

/* Tokens */
%%
"\n"  {return NEWLINE;}
({letra}|{digito}|{especiais})* {return IDENTIFICADOR;}
" " {return SPACE;}
"&" {return ECOMERCIAL;}
"2>" {return MAIOR2;}
"2>>" {return MAIORMAIOR2;}
">>" {return MAIORMAIOR;}
">" {return MAIOR;}
"<" {return MENOR;}
"|" {return PIPE;}
%%

/* Seta a string a ser processada pelo lexer como
 * a recebida pelo parametro
 */
void* setup_lexer_string(char const* line)
{
    YY_BUFFER_STATE buffer  = yy_scan_string(line);
    yy_switch_to_buffer(buffer);

    return buffer;
}

void clean(void* buffer)
{
    yy_delete_buffer((YY_BUFFER_STATE)buffer);
}
