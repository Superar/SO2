#include "command_parser.h"

char* ler_comando()
{
    char *buffer = NULL;
    size_t tamanho_buffer = 0;

    getline(&buffer, &tamanho_buffer, stdin);

    // Substitui o caracter '\n' pelo caracter '\0'
    char* newline = strchr(buffer, '\n');
    if (newline)
    {
        *newline = '\0';
    }

    return buffer;
}

char** parse_comando(char *comando)
{
    int tamanho_buffer = 100, pos = 0;
    char **tokens = malloc(tamanho_buffer * sizeof(char*));
    int token;

    if (!tokens)
    {
        fprintf(stderr, "Erro na alocacao do vetor de tokens\n");
        exit(EXIT_FAILURE);
    }

    setup_lexer_string(comando);
    while ( (token = yylex()) )
    {
        if (token != SPACE)
        {
            char *aux = NULL;
            aux = malloc(yyleng * sizeof(char));
            strcpy(aux, yytext);
            tokens[pos++] = aux;
        }

        // Caso o vetor de tokens nao seja suficiente
        if (pos == tamanho_buffer-1)
        {
            tamanho_buffer += tamanho_buffer;
            tokens = realloc(tokens, tamanho_buffer);
        }
    }
    tokens[pos] = NULL; // Token NULL para saber o final
    
    return tokens;
}

int yywrap() { return 1; }
