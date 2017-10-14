#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command_parser.h"
#include "processes_manager.h"

int executar_comando(char**);

int main(int argc, char const *argv[])
{
    int fechar = 0, pos = 0;
    char *comando;
    char **tokens;

    while (!fechar)
    {
        printf("$ ");
        comando = ler_comando();
        tokens = parse_comando(comando);
        fechar = executar_comando(tokens);
    }
    return EXIT_SUCCESS;
}

int executar_comando(char **tokens)
{
    if (strcmp(tokens[0], "exit") == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
