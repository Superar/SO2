#include "processes_manager.h"

int executar_comando(Comando* comandos)
{
    if (strcmp(comandos->args[0], "exit") == 0)
    {
        return 1;
    }
    else
    {
        process p;
        p.pid = fork();
        p.args = comandos->args;
        int status;

        if (p.pid < 0)
        {
            fprintf(stderr, "Erro na execu%c%co do fork\n", 135, 198);
            exit(EXIT_FAILURE);
        }
        else if (p.pid == 0) // filho
        {
            execvp(p.args[0], p.args);
            exit(EXIT_SUCCESS);
        }
        else // pai
        {
            waitpid(p.pid, &status, 0);
            return 0;
        }
    }
}
