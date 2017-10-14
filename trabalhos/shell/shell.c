#include "command_parser.h"
#include "processes_manager.h"

int main(int argc, char const *argv[])
{
    int fechar = 0;
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
