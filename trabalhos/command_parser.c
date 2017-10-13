#include "command_parser.h"

char* ler_comando()
{
    char *buffer = NULL;
    ssize_t tamanho_buffer = 0;

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
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "Erro na alocacao do vetor de tokens\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(comando, " "); // Primeiro token
    while (token != NULL)
    {
        tokens[pos++] = token;

        // Caso o vetor de tokens nao seja suficiente
        if (pos == tamanho_buffer-1)
        {
            tamanho_buffer += tamanho_buffer;
            tokens = realloc(tokens, tamanho_buffer);
        }

        token = strtok(NULL, " "); // Proximo token
    }
    tokens[pos] = NULL; // Token NULL para saber o final
    return tokens;
}
