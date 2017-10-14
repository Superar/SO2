#include <signal.h>
#include "command_parser.h"
#include "processes_manager.h"

int main(int argc, char const *argv[])
{
    int fechar = 0;
    char *comando;
    char **tokens;

    if (isatty(STDIN_FILENO))
    {
        while (tcgetpgrp(STDIN_FILENO) != getpgrp())
        {
            kill(-getpgrp(), SIGTTIN);
        }

        while (!fechar)
        {
            printf("$ ");
            comando = ler_comando();
            tokens = parse_comando(comando);
            fechar = executar_comando(tokens);
        }
        return EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "Programa precisa ser inicializado em um terminal\n");
        return EXIT_FAILURE;
    }
}
