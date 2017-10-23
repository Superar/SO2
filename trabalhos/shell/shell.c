#include <signal.h>
#include "command_parser.h"
#include "processes_manager.h"

int main(int argc, char const *argv[])
{
    int fechar = 0;
    char *comando_str;
    Comando *comandos;

    if (isatty(STDIN_FILENO))
    {
        while (tcgetpgrp(STDIN_FILENO) != getpgrp())
        {
            kill(-getpgrp(), SIGTTIN);
        }

        while (!fechar)
        {
            printf("$ ");
            comando_str = ler_comando();
            comandos = parse_comando(comando_str);
            fechar = executar_comando(comandos);
        }
        return EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "Programa precisa ser inicializado em um terminal\n");
        return EXIT_FAILURE;
    }
}
