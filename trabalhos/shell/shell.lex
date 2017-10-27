digito  [0-9]
letra   [A-Za-z]
especiais [-./~%*]

%{
#include <string.h>
#include "command_parser.h"
%}

%option nounput
%option noinput

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
